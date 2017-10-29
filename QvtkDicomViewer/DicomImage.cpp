#include "DicomImage.h"



DicomImage::DicomImage()
{
}


DicomImage::~DicomImage()
{
}
std::string DicomImage::getReferencedFileID()
{
	return ReferencedFileID;
}
void DicomImage::setReferencedFileID(std::string value)
{
	ReferencedFileID = value;
}
std::string DicomImage::getInstanceCreationDate()
{
	return InstanceCreationDate;
}
void DicomImage::setInstanceCreationDate(std::string value)
{
	InstanceCreationDate = value;
}
std::string DicomImage::getkpv()
{
	return kpv;
}
void DicomImage::setkpv(std::string value)
{
	kpv = value;
}
std::string DicomImage::getSliceLocation()
{
	return SliceLocation;
}
void DicomImage::setSliceLocation(std::string value)
{
	SliceLocation = value;
}
std::string DicomImage::getPixelSpacing()
{
	return PixelSpacing;
}
void DicomImage::setPixelSpacing(std::string value)
{
	PixelSpacing = value;
}
int DicomImage::getRow()
{
	return row;
}
void DicomImage::setRow(int value)
{
	row = value;
}
int DicomImage::getCol()
{
	return col;
}
void DicomImage::setCol(int value)
{
	col = value;
}