#pragma once
#include "vtkInteractorStyleImage.h"
#include <vtkTextMapper.h>
#include <vtkImageViewer2.h>
#include <sstream>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>

#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>

#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkInteractorStyleImage.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
// needed to easily convert int to std::string
#include <sstream>
class StatusMessage {
public:
	static std::string Format(int slice, int maxSlice) {
		std::stringstream tmp;
		tmp << "Slice Number  " << slice + 1 << "/" << maxSlice + 1;
		return tmp.str();
	}
};

class myVtkInteractorStyleImage :public vtkInteractorStyleImage
{
public:
	static myVtkInteractorStyleImage* New();
	vtkTypeMacro(myVtkInteractorStyleImage, vtkInteractorStyleImage);
	enum MOUSEFUNC
	{
		POINTER,
		ZOOM,
		GRAYLEVEL,
		MOVE
	}MouseFunction;
protected:
	vtkImageViewer2* _ImageViewer;
	vtkTextMapper* _StatusMapper;
	int _Slice;
	int _MinSlice;
	int _MaxSlice;

public:
	void SetImageViewer(vtkImageViewer2* imageViewer);
	void SetStatusMapper(vtkTextMapper* statusMapper);
	void MoveSliceForward();
	void MoveSliceBackward();
	void PlaySlice();//自动播放
	int getSlice();//返回当前的页数
	int getMinSlice();//返回最小页码
	int getMaxSlice();//返回最大页码
	void ResetSliceToMin();//重置到最小页码并重绘
protected:
	virtual void OnKeyDown()override;
	virtual void OnMouseWheelForward()override;
	virtual void OnMouseWheelBackward()override;

	//重载左键的按下和抬起
	virtual void OnLeftButtonDown() override;
	virtual void OnLeftButtonUp() override;
	virtual void OnMiddleButtonDown() override;
	virtual void OnMiddleButtonUp() override;
	virtual void OnRightButtonDown() override;
	virtual void OnRightButtonUp() override;

};

