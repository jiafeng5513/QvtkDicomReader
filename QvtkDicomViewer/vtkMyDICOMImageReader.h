#pragma once
#ifndef vtkMyDICOMImageReader_h
#define vtkMyDICOMImageReader_h
#include "vtkIOImageModule.h" // For export macro
#include "vtkImageReader2.h"
#include "vtk-8.0/DICOMAppHelper.h"
#include "vtk-8.0/DICOMParser.h"
/*
 * 这一段是有意义的注释:
 * 
 * DICOMParser.h和DICOMAppHelper.h这两个类和vtk与itk的交互有关
 * itk和vtk中都有实现,但是实现稍有区别
 * 如果一个程序既要用itk又要用vtk,多半和这两个类有关系
 * 大多数情况下我们不需要去区分到底引用了那个库里面的类
 * 但是目前的情况不一样,我们正在重载vtk中的一个类,那么我们必须确定这个类是我们从vtk 的库中引用出来的
 * 不然有些方法是没有实现的或者是实现不一样的
 * 这个例子能触发resharper的误判,但是VS本身不会误判,
 * 这提醒我们在编制带有交叉引用请款的C++程序的时候,不要过分依赖于重构工具的提示而要依赖于文件对比和直接代码审阅
 */
/*
 * 在这里首先吐槽一下VTK官方实现的"vtkDicomImageReadere"
 * 这个类有一个明显的漏洞:
 * 我们只能从文件名或者文件夹的名字来初始化这个类
 * 这要求我们要不然就是只能读一张图,要不然就是读一个文件夹下面的所有图,
 * 否则,我们要读的这些文件的名字必须有某些规则,
 * 如果上述三个条件都不满足,这个类就没法用
 * 实际上我只是要求他有一个"SetFileNames"的方法,然而不幸得是,这个方法在他的父类ImageReader2中给了实现,
 * 在他自己上却没有实现,
 * 算了反正也不麻烦本座自己写一个行吧,我错了行不行....
 * 日后待老子闲下来一点了去你们github上提意见
 */


class vtkDICOMImageReaderVector;
class DICOMParser;
class DICOMAppHelper;

class /*VTKIOIMAGE_EXPORT*/ vtkMyDICOMImageReader:public vtkImageReader2
{
public:
	//@{
	/**
	* 静态构造工厂方法,可以配合智能指针使用
	*/
	static vtkMyDICOMImageReader *New();
	vtkTypeMacro(vtkMyDICOMImageReader, vtkImageReader2);
	//@}
	unsigned long getImageDateLength();//获取图片数据的长度,如果长度为0,说明这不是有效的DICOM图片文件
	/**
	* 调试日志输出
	*/
	void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;

	//@{
	/**
	 *从单个文件初始化,注意所有和设置文件名相关的方法是排他的
	*/
	void SetFileName(const char* fn) VTK_OVERRIDE
	{
		delete[] this->DirectoryName;
		delete[] this->FileName;
		this->DirectoryName = NULL;
		this->FileName = NULL;
		this->vtkImageReader2::SetFileName(fn);
	}
	//@}

	/**
	* Set the directory name for the reader to look in for DICOM
	* files. If this method is used, the reader will try to find
	* all the DICOM files in a directory. It will select the subset
	* corresponding to the first series UID it stumbles across and
	* it will try to build an ordered volume from them based on
	* the slice number. The volume building will be upgraded to
	* something more sophisticated in the future.
	*/
	void SetDirectoryName(const char* dn);

	/*
	 * 重载父类中的SetFileNames,突破限制
	 * 注意:
	 * 1.慎用此方法,目前只允许在一种情况下动用这个方法:从零散文件初始化reader并进行三维重建
	 * 2.注意函数的输入,为了形成覆盖效果,这里使用了vtkStringArray,注意使用智能指针
	 */
	void SetFileNames(vtkStringArray*) override;
	//@{
	/**
	* Returns the directory name.
	*/
	vtkGetStringMacro(DirectoryName);
	//@}

	/**
	* Returns the pixel spacing (in X, Y, Z).
	* Note: if there is only one slice, the Z spacing is set to the slice
	* thickness. If there is more than one slice, it is set to the distance
	* between the first two slices.
	*/
	double* GetPixelSpacing();

	/**
	* Returns the image width.
	*/
	int GetWidth();

	/**
	* Returns the image height.
	*/
	int GetHeight();

	/**
	* Get the (DICOM) x,y,z coordinates of the first pixel in the
	* image (upper left hand corner) of the last image processed by the
	* DICOMParser
	*/
	float* GetImagePositionPatient();

	/**
	* Get the (DICOM) directions cosines. It consist of the components
	* of the first two vectors. The third vector needs to be computed
	* to form an orthonormal basis.
	*/
	float* GetImageOrientationPatient();

	/**
	* Get the number of bits allocated for each pixel in the file.
	*/
	int GetBitsAllocated();

	/**
	* Get the pixel representation of the last image processed by the
	* DICOMParser. A zero is a unsigned quantity.  A one indicates a
	* signed quantity
	*/
	int GetPixelRepresentation();

	/**
	* Get the number of components of the image data for the last
	* image processed.
	*/
	int GetNumberOfComponents();

	/**
	* Get the transfer syntax UID for the last image processed.
	*/
	const char* GetTransferSyntaxUID();

	/**
	* Get the rescale slope for the pixel data.
	*/
	float GetRescaleSlope();

	/**
	* Get the rescale offset for the pixel data.
	*/
	float GetRescaleOffset();

	/**
	* Get the patient name for the last image processed.
	*/
	const char* GetPatientName();

	/**
	* Get the study uid for the last image processed.
	*/
	const char* GetStudyUID();

	/**
	* Get the Study ID for the last image processed.
	*/
	const char* GetStudyID();

	/**
	* Get the gantry angle for the last image processed.
	*/
	float GetGantryAngle();

	//
	// Can I read the file?
	//
	int CanReadFile(const char* fname) VTK_OVERRIDE;

	//
	// What file extensions are supported?
	//
	const char* GetFileExtensions() VTK_OVERRIDE
	{
		return ".dcm";
	}

	/**
	* Return a descriptive name for the file format that might be useful in a GUI.
	*/
	const char* GetDescriptiveName() VTK_OVERRIDE
	{
		return "DICOM";
	}

protected:
	//
	// Setup the volume size
	//
	void SetupOutputInformation(int num_slices);

	void ExecuteInformation() VTK_OVERRIDE;
	void ExecuteDataWithInformation(vtkDataObject *out, vtkInformation *outInfo) VTK_OVERRIDE;

	//
	// Constructor
	//
	vtkMyDICOMImageReader();

	//
	// Destructor
	//
	~vtkMyDICOMImageReader() VTK_OVERRIDE;

	//
	// Instance of the parser used to parse the file.
	//
	DICOMParser* Parser;

	//
	// Instance of the callbacks that get the data from the file.
	//
	DICOMAppHelper* AppHelper;

	//
	// vtkDICOMImageReaderVector wants to be a PIMPL and it will be, but not quite yet.
	//
	vtkDICOMImageReaderVector* DICOMFileNames;
	char* DirectoryName;

	char* PatientName;
	char* StudyUID;
	char* StudyID;
	char* TransferSyntaxUID;

	// DICOMFileNames accessor methods for subclasses:
	int GetNumberOfDICOMFileNames();
	const char* GetDICOMFileName(int index);
private:
	vtkMyDICOMImageReader(const vtkMyDICOMImageReader&) VTK_DELETE_FUNCTION;
	void operator=(const vtkMyDICOMImageReader&) VTK_DELETE_FUNCTION;

};
#endif