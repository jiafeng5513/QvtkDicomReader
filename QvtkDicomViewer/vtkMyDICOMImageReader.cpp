#include "vtkMyDICOMImageReader.h"



/*=========================================================================

Program:   Visualization Toolkit
Module:    vtkMyDICOMImageReader.cxx

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkDirectory.h"
#include "vtkDataArray.h"
#include "vtkImageData.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkErrorCode.h"
#include "vtkStringArray.h"

#include <vtksys/SystemTools.hxx>

#include <vector>
#include <string>

#include "vtk-8.0/DICOMAppHelper.h"
#include "vtk-8.0/DICOMParser.h"

vtkStandardNewMacro(vtkMyDICOMImageReader);

class vtkDICOMImageReaderVector : public std::vector<std::string>
{

};

//----------------------------------------------------------------------------
vtkMyDICOMImageReader::vtkMyDICOMImageReader()
{
	this->Parser = new DICOMParser();
	this->AppHelper = new DICOMAppHelper();
	this->DirectoryName = NULL;
	this->PatientName = NULL;
	this->StudyUID = NULL;
	this->StudyID = NULL;
	this->TransferSyntaxUID = NULL;
	this->DICOMFileNames = new vtkDICOMImageReaderVector();
}

//----------------------------------------------------------------------------
vtkMyDICOMImageReader::~vtkMyDICOMImageReader()
{
	delete this->Parser;
	delete this->AppHelper;
	delete this->DICOMFileNames;

	delete[] this->DirectoryName;
	delete[] this->PatientName;
	delete[] this->StudyUID;
	delete[] this->StudyID;
	delete[] this->TransferSyntaxUID;
}

//----------------------------------------------------------------------------
void vtkMyDICOMImageReader::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
	if (this->DirectoryName)
	{
		os << "DirectoryName : " << this->DirectoryName << "\n";
	}
	else
	{
		os << "DirectoryName : (NULL)" << "\n";
	}
	if (this->FileName)
	{
		os << "FileName : " << this->FileName << "\n";
	}
	else
	{
		os << "FileName : (NULL)" << "\n";
	}

}

//----------------------------------------------------------------------------
int vtkMyDICOMImageReader::CanReadFile(const char* fname)
{
	bool canOpen = this->Parser->OpenFile((const char*)fname);
	if (!canOpen)
	{
		vtkErrorMacro("DICOMParser couldn't open : " << fname);
		return 0;
	}
	bool canRead = this->Parser->IsDICOMFile();
	if (canRead)
	{
		return 1;
	}
	else
	{
		vtkWarningMacro("DICOMParser couldn't parse : " << fname);
		return 0;
	}
}

//----------------------------------------------------------------------------
void vtkMyDICOMImageReader::ExecuteInformation()
{
	if (this->FileName == NULL && this->DirectoryName == NULL&&this->FileNames == NULL)
	{
		return;//此时应该探测FileNames中有没有东西
	}
	/*
	 * 下面是分两种情况处理
	 * 情况一:数据源在FileName中
	 * 情况二:数据在DirectoryName中
	 * 实际上我们还应该看看FileNames中有没有货
	 */
	if (this->FileName)//如果用单张图片
	{
		vtksys::SystemTools::Stat_t fs;
		if (vtksys::SystemTools::Stat(this->FileName, &fs))
		{
			vtkErrorMacro("Unable to open file " << this->FileName);
			return;
		}

		this->DICOMFileNames->clear();
		this->AppHelper->Clear();
		this->Parser->ClearAllDICOMTagCallbacks();

		this->Parser->OpenFile(this->FileName);
		this->AppHelper->RegisterCallbacks(this->Parser);

		this->Parser->ReadHeader();
		this->SetupOutputInformation(1);
	}
	else if (this->DirectoryName)
	{
		vtkDirectory* dir = vtkDirectory::New();
		int opened = dir->Open(this->DirectoryName);
		if (!opened)
		{
			vtkErrorMacro("Couldn't open " << this->DirectoryName);
			dir->Delete();
			return;
		}
		vtkIdType numFiles = dir->GetNumberOfFiles();

		vtkDebugMacro(<< "There are " << numFiles << " files in the directory.");

		this->DICOMFileNames->clear();
		this->AppHelper->Clear();

		for (vtkIdType i = 0; i < numFiles; i++)
		{
			if (strcmp(dir->GetFile(i), ".") == 0 ||
				strcmp(dir->GetFile(i), "..") == 0)
			{
				continue;
			}

			std::string fileString = this->DirectoryName;
			fileString += "/";
			fileString += dir->GetFile(i);

			int val = this->CanReadFile(fileString.c_str());

			if (val == 1)
			{
				vtkDebugMacro(<< "Adding " << fileString.c_str() << " to DICOMFileNames.");
				this->DICOMFileNames->push_back(fileString);
			}
			else
			{
				vtkDebugMacro(<< fileString.c_str() << " - DICOMParser CanReadFile returned : " << val);
			}

		}
		std::vector<std::string>::iterator iter;

		for (iter = this->DICOMFileNames->begin();
			iter != this->DICOMFileNames->end();
			++iter)
		{
			const char* fn = iter->c_str();
			vtkDebugMacro(<< "Trying : " << fn);

			bool couldOpen = this->Parser->OpenFile(fn);
			if (!couldOpen)
			{
				dir->Delete();
				return;
			}

			//
			this->Parser->ClearAllDICOMTagCallbacks();
			this->AppHelper->RegisterCallbacks(this->Parser);

			this->Parser->ReadHeader();
			this->Parser->CloseFile();

			vtkDebugMacro(<< "File name : " << fn);
			vtkDebugMacro(<< "Slice number : " << this->AppHelper->GetSliceNumber());
		}

		std::vector<std::pair<float, std::string> > sortedFiles;

		this->AppHelper->GetImagePositionPatientFilenamePairs(sortedFiles, false);
		this->SetupOutputInformation(static_cast<int>(sortedFiles.size()));

		//this->AppHelper->OutputSeries();

		if (sortedFiles.size() > 0)
		{
			this->DICOMFileNames->clear();
			std::vector<std::pair<float, std::string> >::iterator siter;
			for (siter = sortedFiles.begin();
				siter != sortedFiles.end();
				++siter)
			{
				vtkDebugMacro(<< "Sorted filename : " << (*siter).second.c_str());
				vtkDebugMacro(<< "Adding file " << (*siter).second.c_str() << " at slice : " << (*siter).first);
				this->DICOMFileNames->push_back((*siter).second);
			}
		}
		else
		{
			vtkErrorMacro(<< "Couldn't get sorted files. Slices may be in wrong order!");
		}
		dir->Delete();
	}
	else if (this->FileNames)//如果现在Filenames非空,从这里初始化
	{
		//vtkDirectory* dir = vtkDirectory::New();
		//int opened = dir->Open(this->DirectoryName);
		//if (!opened)
		//{
		//	vtkErrorMacro("Couldn't open " << this->DirectoryName);
		//	dir->Delete();
		//	return;
		//}
		////vtkIdType numFiles = dir->GetNumberOfFiles();
		vtkIdType numFiles = FileNames->GetSize();//由于不明原因,这个size是不可信的,往往要多一些,

		vtkDebugMacro(<< "There are " << numFiles << " files in the directory.");

		this->DICOMFileNames->clear();
		this->AppHelper->Clear();

		for (vtkIdType i = 0; i < numFiles; i++)
		{
			//if (strcmp(dir->GetFile(i), ".") == 0 ||
			//	strcmp(dir->GetFile(i), "..") == 0)
			//{
			//	continue;
			//}

			//std::string fileString = this->DirectoryName;
			//fileString += "/";
			//fileString += dir->GetFile(i);

			std::string fileString = FileNames->GetValue(i);
			if (fileString=="")
			{
				continue;//跳过空值,以免由于size的冗余大小导致的CanReadFile报运行时错误
			}
			int val = this->CanReadFile(fileString.c_str());

			if (val == 1)
			{
				vtkDebugMacro(<< "Adding " << fileString.c_str() << " to DICOMFileNames.");
				this->DICOMFileNames->push_back(fileString);
			}
			else
			{
				vtkDebugMacro(<< fileString.c_str() << " - DICOMParser CanReadFile returned : " << val);
			}

		}
		std::vector<std::string>::iterator iter;

		for (iter = this->DICOMFileNames->begin();
			iter != this->DICOMFileNames->end();
			++iter)
		{
			const char* fn = iter->c_str();
			vtkDebugMacro(<< "Trying : " << fn);

			bool couldOpen = this->Parser->OpenFile(fn);
			if (!couldOpen)
			{
				return;//突然死亡法,一个失败文件直接导致整个函数退掉
			}

			//
			this->Parser->ClearAllDICOMTagCallbacks();
			this->AppHelper->RegisterCallbacks(this->Parser);

			this->Parser->ReadHeader();
			this->Parser->CloseFile();

			vtkDebugMacro(<< "File name : " << fn);
			vtkDebugMacro(<< "Slice number : " << this->AppHelper->GetSliceNumber());
		}

		std::vector<std::pair<float, std::string> > sortedFiles;

		this->AppHelper->GetImagePositionPatientFilenamePairs(sortedFiles, false);
		this->SetupOutputInformation(static_cast<int>(sortedFiles.size()));

		//this->AppHelper->OutputSeries();

		if (sortedFiles.size() > 0)
		{
			this->DICOMFileNames->clear();
			std::vector<std::pair<float, std::string> >::iterator siter;
			for (siter = sortedFiles.begin();
				siter != sortedFiles.end();
				++siter)
			{
				vtkDebugMacro(<< "Sorted filename : " << (*siter).second.c_str());
				vtkDebugMacro(<< "Adding file " << (*siter).second.c_str() << " at slice : " << (*siter).first);
				this->DICOMFileNames->push_back((*siter).second);
			}
		}
		else
		{
			vtkErrorMacro(<< "Couldn't get sorted files. Slices may be in wrong order!");
		}
		//dir->Delete();
	}

}

//----------------------------------------------------------------------------
void vtkMyDICOMImageReader::ExecuteDataWithInformation(vtkDataObject *output,
	vtkInformation *outInfo)
{
	vtkImageData *data = this->AllocateOutputData(output, outInfo);

	if (!this->FileName && this->DICOMFileNames->size() == 0)
	{
		vtkErrorMacro(<< "Either a filename was not specified or the specified directory does not contain any DICOM images.");
		this->SetErrorCode(vtkErrorCode::NoFileNameError);
		return;
	}

	data->GetPointData()->GetScalars()->SetName("DICOMImage");

	this->ComputeDataIncrements();

	if (this->FileName)
	{
		vtkDebugMacro(<< "Single file : " << this->FileName);
		this->Parser->ClearAllDICOMTagCallbacks();
		this->Parser->OpenFile(this->FileName);
		this->AppHelper->Clear();
		this->AppHelper->RegisterCallbacks(this->Parser);
		this->AppHelper->RegisterPixelDataCallback(this->Parser);

		this->Parser->ReadHeader();

		void* imgData = NULL;
		DICOMParser::VRTypes dataType;
		unsigned long imageDataLength;

		this->AppHelper->GetImageData(imgData, dataType, imageDataLength);
		if (!imageDataLength)
		{
			vtkErrorMacro(<< "There was a problem retrieving data from: " << this->FileName);
			this->SetErrorCode(vtkErrorCode::FileFormatError);
			return;
		}

		void* buffer = data->GetScalarPointer();
		if (buffer == NULL)
		{
			vtkErrorMacro(<< "No memory allocated for image data!");
			return;
		}
		// DICOM stores the upper left pixel as the first pixel in an
		// image. VTK stores the lower left pixel as the first pixel in
		// an image.  Need to flip the data.
		vtkIdType rowLength;
		rowLength = this->DataIncrements[1];
		unsigned char *b = (unsigned char *)buffer;
		unsigned char *iData = (unsigned char *)imgData;
		iData += (imageDataLength - rowLength); // beginning of last row
		for (int i = 0; i < this->AppHelper->GetHeight(); ++i)
		{
			memcpy(b, iData, rowLength);
			b += rowLength;
			iData -= rowLength;
		}
	}
	else if (this->DICOMFileNames->size() > 0)
	{
		vtkDebugMacro(<< "Multiple files (" << static_cast<int>(this->DICOMFileNames->size()) << ")");
		this->Parser->ClearAllDICOMTagCallbacks();
		this->AppHelper->Clear();
		this->AppHelper->RegisterCallbacks(this->Parser);
		this->AppHelper->RegisterPixelDataCallback(this->Parser);

		void* buffer = data->GetScalarPointer();
		if (buffer == NULL)
		{
			vtkErrorMacro(<< "No memory allocated for image data!");
			return;
		}

		std::vector<std::string>::iterator fiter;

		int count = 0;
		vtkIdType numFiles = static_cast<int>(this->DICOMFileNames->size());

		for (fiter = this->DICOMFileNames->begin();
			fiter != this->DICOMFileNames->end();
			++fiter)
		{
			count++;
			const char *file = fiter->c_str();
			vtkDebugMacro(<< "File : " << file);
			this->Parser->OpenFile(file);
			this->Parser->ReadHeader();

			void* imgData = NULL;
			DICOMParser::VRTypes dataType;
			unsigned long imageDataLengthInBytes;

			this->AppHelper->GetImageData(imgData, dataType, imageDataLengthInBytes);
			if (!imageDataLengthInBytes)
			{
				vtkErrorMacro(<< "There was a problem retrieving data from: " << file);
				this->SetErrorCode(vtkErrorCode::FileFormatError);
				return;
			}

			// DICOM stores the upper left pixel as the first pixel in an
			// image. VTK stores the lower left pixel as the first pixel in
			// an image.  Need to flip the data.
			vtkIdType rowLength;
			rowLength = this->DataIncrements[1];
			unsigned char *b = (unsigned char *)buffer;
			unsigned char *iData = (unsigned char *)imgData;
			iData += (imageDataLengthInBytes - rowLength); // beginning of last row
			for (int i = 0; i < this->AppHelper->GetHeight(); ++i)
			{
				memcpy(b, iData, rowLength);
				b += rowLength;
				iData -= rowLength;
			}
			buffer = ((char*)buffer) + imageDataLengthInBytes;

			this->UpdateProgress(float(count) / float(numFiles));
			int len = static_cast<int> (strlen((const char*)(*fiter).c_str()));
			char* filename = new char[len + 1];
			strcpy(filename, (const char*)(*fiter).c_str());
			this->SetProgressText(filename);
			delete[] filename;
		}
	}
}

//----------------------------------------------------------------------------
void vtkMyDICOMImageReader::SetupOutputInformation(int num_slices)
{
	int width = this->AppHelper->GetWidth();
	int height = this->AppHelper->GetHeight();
	int bit_depth = this->AppHelper->GetBitsAllocated();
	int num_comp = this->AppHelper->GetNumberOfComponents();

	this->DataExtent[0] = 0;
	this->DataExtent[1] = width - 1;
	this->DataExtent[2] = 0;
	this->DataExtent[3] = height - 1;
	this->DataExtent[4] = 0;
	this->DataExtent[5] = num_slices - 1;

	bool isFloat = this->AppHelper->RescaledImageDataIsFloat();

	bool sign = this->AppHelper->RescaledImageDataIsSigned();

	if (isFloat)
	{
		this->SetDataScalarTypeToFloat();
	}
	else if (bit_depth <= 8)
	{
		this->SetDataScalarTypeToUnsignedChar();
	}
	else
	{
		if (sign)
		{
			this->SetDataScalarTypeToShort();
		}
		else
		{
			this->SetDataScalarTypeToUnsignedShort();
		}
	}
	this->SetNumberOfScalarComponents(num_comp);

	this->GetPixelSpacing();

	this->vtkImageReader2::ExecuteInformation();
}

//----------------------------------------------------------------------------
void vtkMyDICOMImageReader::SetDirectoryName(const char* dn)
{
	vtkDebugMacro(<< this->GetClassName() << " (" << this <<
		"): setting DirectoryName to " << (dn ? dn : "(null)"));
	if (this->DirectoryName == NULL && dn == NULL)
	{
		return;
	}
	delete[] this->FileName;
	this->FileName = NULL;
	if (this->DirectoryName && dn && (!strcmp(this->DirectoryName, dn)))
	{
		return;
	}
	delete[] this->DirectoryName;
	if (dn)
	{
		this->DirectoryName = new char[strlen(dn) + 1];
		strcpy(this->DirectoryName, dn);
	}
	else
	{
		this->DirectoryName = NULL;
	}
	this->Modified();
}
/*
 * 听说这个函数能解决全人类的一切烦恼?(+1s....)
 */
void vtkMyDICOMImageReader::SetFileNames(vtkStringArray*filenames_array)
{
	/*
	 * 要解决这个问题实际上分两步
	 * 第一步,这个地方对来的数据给一个完整性验证,然后怼到一个什么地方
	 * 第二步,ExecuteInformation()这个函数里面有数据的中转,加一个出口
	 * 然后我觉得应该把日志那块改一下,printself这个函数的实现有点莫名其妙,
	 * 既然要改就使劲改哈哈哈哈
	 * 
	 * 
	 * 备注:全局检索 DirectoryName 
	 */
	if (filenames_array == this->FileNames)
	{
		return;
	}
	if (this->FileNames)
	{
		this->FileNames->Delete();
		this->FileNames = 0;
	}
	if (filenames_array)
	{
		this->FileNames = filenames_array;
		this->FileNames->Register(this);
		if (this->FileNames->GetNumberOfValues() > 0)
		{
			this->DataExtent[4] = 0;
			this->DataExtent[5] = this->FileNames->GetNumberOfValues() - 1;
		}
		delete[] this->FilePrefix;
		this->FilePrefix = NULL;
		delete[] this->FileName;
		this->FileName = NULL;
	}

	this->Modified();


}

//----------------------------------------------------------------------------
double* vtkMyDICOMImageReader::GetPixelSpacing()
{
	std::vector<std::pair<float, std::string> > sortedFiles;

	this->AppHelper->GetImagePositionPatientFilenamePairs(sortedFiles, false);

	float* spacing = this->AppHelper->GetPixelSpacing();
	this->DataSpacing[0] = spacing[0];
	this->DataSpacing[1] = spacing[1];

	if (sortedFiles.size() > 1)
	{
		std::pair<float, std::string> p1 = sortedFiles[0];
		std::pair<float, std::string> p2 = sortedFiles[1];
		this->DataSpacing[2] = fabs(p1.first - p2.first);
	}
	else
	{
		this->DataSpacing[2] = spacing[2];
	}

	return this->DataSpacing;
}

//----------------------------------------------------------------------------
int vtkMyDICOMImageReader::GetWidth()
{
	return this->AppHelper->GetWidth();
}

//----------------------------------------------------------------------------
int vtkMyDICOMImageReader::GetHeight()
{
	return this->AppHelper->GetHeight();
}

//----------------------------------------------------------------------------
float* vtkMyDICOMImageReader::GetImagePositionPatient()
{
	return this->AppHelper->GetImagePositionPatient();
}

//----------------------------------------------------------------------------
float* vtkMyDICOMImageReader::GetImageOrientationPatient()
{
	return this->AppHelper->GetImageOrientationPatient();
}

//----------------------------------------------------------------------------
int vtkMyDICOMImageReader::GetBitsAllocated()
{
	return this->AppHelper->GetBitsAllocated();
}

//----------------------------------------------------------------------------
int vtkMyDICOMImageReader::GetPixelRepresentation()
{
	return this->AppHelper->GetPixelRepresentation();
}

//----------------------------------------------------------------------------
int vtkMyDICOMImageReader::GetNumberOfComponents()
{
	return this->AppHelper->GetNumberOfComponents();
}

//----------------------------------------------------------------------------
const char* vtkMyDICOMImageReader::GetTransferSyntaxUID()
{
	std::string tmp = this->AppHelper->GetTransferSyntaxUID();

	delete[] this->TransferSyntaxUID;
	this->TransferSyntaxUID = new char[tmp.length() + 1];
	strcpy(this->TransferSyntaxUID, tmp.c_str());
	this->TransferSyntaxUID[tmp.length()] = '\0';

	return this->TransferSyntaxUID;
}

//----------------------------------------------------------------------------
float vtkMyDICOMImageReader::GetRescaleSlope()
{
	return this->AppHelper->GetRescaleSlope();
}

//----------------------------------------------------------------------------
float vtkMyDICOMImageReader::GetRescaleOffset()
{
	return this->AppHelper->GetRescaleOffset();
}

//----------------------------------------------------------------------------
const char* vtkMyDICOMImageReader::GetPatientName()
{
	std::string tmp = this->AppHelper->GetPatientName();

	delete[] this->PatientName;
	this->PatientName = new char[tmp.length() + 1];
	strcpy(this->PatientName, tmp.c_str());
	this->PatientName[tmp.length()] = '\0';

	return this->PatientName;
}

//----------------------------------------------------------------------------
const char* vtkMyDICOMImageReader::GetStudyUID()
{
	std::string tmp = this->AppHelper->GetStudyUID();

	delete[] this->StudyUID;
	this->StudyUID = new char[tmp.length() + 1];
	strcpy(this->StudyUID, tmp.c_str());
	this->StudyUID[tmp.length()] = '\0';

	return this->StudyUID;
}

//----------------------------------------------------------------------------
const char* vtkMyDICOMImageReader::GetStudyID()
{
	std::string tmp = this->AppHelper->GetStudyID();

	delete[] this->StudyID;
	this->StudyID = new char[tmp.length() + 1];
	strcpy(this->StudyID, tmp.c_str());
	this->StudyID[tmp.length()] = '\0';

	return this->StudyID;
}

//----------------------------------------------------------------------------
float vtkMyDICOMImageReader::GetGantryAngle()
{
	return this->AppHelper->GetGantryAngle();
}

//----------------------------------------------------------------------------
int vtkMyDICOMImageReader::GetNumberOfDICOMFileNames()
{
	return static_cast<int>(this->DICOMFileNames->size());
}

//----------------------------------------------------------------------------
const char* vtkMyDICOMImageReader::GetDICOMFileName(int index)
{
	if (index >= 0 && index < this->GetNumberOfDICOMFileNames())
	{
		return (*this->DICOMFileNames)[index].c_str();
	}
	return 0;
}

