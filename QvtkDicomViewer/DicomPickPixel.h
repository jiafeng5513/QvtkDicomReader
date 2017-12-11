#pragma once
#include <head_all.h>
class MyStyle : public vtkInteractorStyleImage
{
public:
	static MyStyle* New();

	//vtkTypeMacro(MyStyle, vtkInteractorStyleImage);

	virtual void OnLeftButtonDown();
};
class vtkImageInteractionCallback : public vtkCommand
{
public:

	static vtkImageInteractionCallback *New();

	vtkImageInteractionCallback();

	~vtkImageInteractionCallback();

	void SetPicker(vtkPropPicker *picker);

	void SetAnnotation(vtkCornerAnnotation *annotation);

	void SetViewer(vtkImageViewer2 *viewer);
	
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


};

int pickpixel(int count, char* argv[], QVTKWidget *qvtk, std::string dir);