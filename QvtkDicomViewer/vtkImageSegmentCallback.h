#pragma once
#include <head_all.h>

enum  SegmentFunc
{
	NULL_Seg,
	Seg_connectedthres,
	Seg_ostu,
	Seg_neighconnected,
	Seg_confidconnected,
	Seg_waterseg,
	Seg_fastmarching,
	Seg_shapedectection,
	Seg_gibblsprior
};

class MyStyle : public vtkInteractorStyleImage
{
public:
	static MyStyle* New();
	void OnLeftButtonDown() override;
};
class vtkImageSegmentCallback : public vtkCommand
{
public:
	static vtkImageSegmentCallback *New();
	vtkImageSegmentCallback();
	~vtkImageSegmentCallback();
	void SetPicker(vtkPropPicker *picker);
	void SetAnnotation(vtkCornerAnnotation *annotation);
	void SetViewer(vtkImageViewer2 *viewer);
	void SetDir(std::string dir);
	void SetQvtk(QVTKWidget* qvtk);
	void SetCount(int count);
	virtual void Execute(vtkObject *, unsigned long event, void *);

private:
	// Pointer to the viewer
	vtkImageViewer2 *Viewer;
	// Pointer to the picker
	vtkPropPicker *Picker;
	// Pointer to the annotation
	vtkCornerAnnotation *Annotation;
	// Interpolator
	vtkPointData* PointData;
	//Qvtk
	QVTKWidget* qvtk;
	int count;
	//std::string  dir;
};

int pickpixel(int count, char* argv[], QVTKWidget *qvtk, std::string dir);