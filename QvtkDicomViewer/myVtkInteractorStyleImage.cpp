#include "myVtkInteractorStyleImage.h"

#include "QvtkDicomViewer.h"

#include "vtkCallbackCommand.h"
#include <vtkRenderWindowInteractor.h>
#include <vtkImageViewer2.h>
#include <vtkTextMapper.h>


void myVtkInteractorStyleImage::SetImageViewer(vtkImageViewer2* imageViewer)
{
	_ImageViewer = imageViewer;
	_MinSlice = imageViewer->GetSliceMin();
	_MaxSlice = imageViewer->GetSliceMax();
	_Slice = _MinSlice;
	//cout << "Slicer: Min = " << _MinSlice << ", Max = " << _MaxSlice << std::endl;
}

void myVtkInteractorStyleImage::SetStatusMapper(vtkTextMapper* statusMapper)
{
	_StatusMapper = statusMapper;
}

void myVtkInteractorStyleImage::MoveSliceForward()
{
	if (_Slice < _MaxSlice)
	{
		_Slice += 1;
		//cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
		_ImageViewer->SetSlice(_Slice);
		std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
		_StatusMapper->SetInput(msg.c_str());
		OnUpdate();
		_ImageViewer->Render();
	}
}

void myVtkInteractorStyleImage::MoveSliceBackward()
{
	if (_Slice > _MinSlice)
	{
		_Slice -= 1;
		//cout << "MoveSliceBackward::Slice = " << _Slice << std::endl;
		_ImageViewer->SetSlice(_Slice);
		std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
		_StatusMapper->SetInput(msg.c_str());
		OnUpdate();
		_ImageViewer->Render();
	}
}
/*
 * 每次需要更新的时候,调用这个函数
 */
void myVtkInteractorStyleImage::OnUpdate()
{
	///TODO:添加更新操作
	if(funcinupdate!=NULL)
	{
		funcinupdate();
	}
}
//添加函数指针
void myVtkInteractorStyleImage::AddUpdate(void(*pfun)(void))
{
	//TODO:添加函数指针
	funcinupdate = pfun;
}

void myVtkInteractorStyleImage::OnKeyDown()
{
	std::string key = this->GetInteractor()->GetKeySym();
	if (key.compare("Up") == 0)
	{
		//cout << "Up arrow key was pressed." << endl;
		MoveSliceForward();
	}
	else if (key.compare("Down") == 0)
	{
		//cout << "Down arrow key was pressed." << endl;
		MoveSliceBackward();
	}
	// forward event
	vtkInteractorStyleImage::OnKeyDown();
}

void myVtkInteractorStyleImage::OnMouseWheelForward()
{
	//std::cout << "Scrolled mouse wheel forward." << std::endl;
	MoveSliceForward();
	// don't forward events, otherwise the image will be zoomed 
	// in case another interactorstyle is used (e.g. trackballstyle, ...)
	// vtkInteractorStyleImage::OnMouseWheelForward();
}

void myVtkInteractorStyleImage::OnMouseWheelBackward()
{
	//std::cout << "Scrolled mouse wheel backward." << std::endl;
	if (_Slice > _MinSlice)
	{
		MoveSliceBackward();
	}
	// don't forward events, otherwise the image will be zoomed 
	// in case another interactorstyle is used (e.g. trackballstyle, ...)
	// vtkInteractorStyleImage::OnMouseWheelBackward();
}
/*
 * 鼠标左键按下
 */
void myVtkInteractorStyleImage::OnLeftButtonDown()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	this->FindPokedRenderer(x, y);

	switch (MouseFunction)
	{
	case POINTER:
		//此时没有功能
		break;
	case ZOOM:
		//缩放,原右键功能
		if (this->CurrentRenderer == NULL)
		{
			return;
		}
		// Redefine this button + shift to handle pick
		this->GrabFocus(this->EventCallbackCommand);
		if (this->Interactor->GetShiftKey())
		{
			this->StartPick();
		}
		else if (this->InteractionMode == VTKIS_IMAGE3D &&
			this->Interactor->GetControlKey())
		{
			this->StartSlice();
		}
		else if (this->InteractionMode == VTKIS_IMAGE_SLICING &&
			this->Interactor->GetControlKey())
		{
			this->StartSpin();
		}

		// The rest of the button + key combinations remain the same

		else
		{
			this->Superclass::OnRightButtonDown();
		}
		break;
	case GRAYLEVEL:
		//灰阶,原左键功能
		if (this->CurrentRenderer == NULL)
		{
			return;
		}
		// Redefine this button to handle window/level
		this->GrabFocus(this->EventCallbackCommand);
		if (!this->Interactor->GetShiftKey() && !this->Interactor->GetControlKey())
		{
			this->WindowLevelStartPosition[0] = x;
			this->WindowLevelStartPosition[1] = y;
			this->StartWindowLevel();
		}
		// If shift is held down, do a rotation
		else if (this->InteractionMode == VTKIS_IMAGE3D &&
			this->Interactor->GetShiftKey())
		{
			this->StartRotate();
		}
		// If ctrl is held down in slicing mode, slice the image
		else if (this->InteractionMode == VTKIS_IMAGE_SLICING &&
			this->Interactor->GetControlKey())
		{
			this->StartSlice();
		}
		// The rest of the button + key combinations remain the same
		else
		{
			this->Superclass::OnLeftButtonDown();
		}
		break;
	case MOVE:
		//平移,原中键按下功能
		this->FindPokedRenderer(this->Interactor->GetEventPosition()[0],
			this->Interactor->GetEventPosition()[1]);
		if (this->CurrentRenderer == NULL)
		{
			return;
		}
		// If shift is held down, change the slice
		if ((this->InteractionMode == VTKIS_IMAGE3D ||
			this->InteractionMode == VTKIS_IMAGE_SLICING) &&
			this->Interactor->GetShiftKey())
		{
			this->StartSlice();
		}
		// The rest of the button + key combinations remain the same
		else
		{
			this->Superclass::OnMiddleButtonDown();
		}
		break;
	default:
		break;
	}
	
}
/*
 * 左键抬起
 */
void myVtkInteractorStyleImage::OnLeftButtonUp()
{
	switch (MouseFunction)
	{
	case POINTER:
		break;
	case ZOOM:
		switch (this->State)
		{
		case VTKIS_PICK:
			this->EndPick();
			if (this->Interactor)
			{
				this->ReleaseFocus();
			}
			break;

		case VTKIS_SLICE:
			this->EndSlice();
			if (this->Interactor)
			{
				this->ReleaseFocus();
			}
			break;

		case VTKIS_SPIN:
			if (this->Interactor)
			{
				this->EndSpin();
			}
			break;
		}
		// Call parent to handle all other states and perform additional work
		this->Superclass::OnRightButtonUp();
		break;
	case GRAYLEVEL:
		switch (this->State)
		{
		case VTKIS_WINDOW_LEVEL:
			this->EndWindowLevel();
			if (this->Interactor)
			{
				this->ReleaseFocus();
			}
			break;

		case VTKIS_SLICE:
			this->EndSlice();
			if (this->Interactor)
			{
				this->ReleaseFocus();
			}
			break;
		}
		// Call parent to handle all other states and perform additional work
		this->Superclass::OnLeftButtonUp();
		break;
	case MOVE:
		switch (this->State)
		{
		case VTKIS_SLICE:
			this->EndSlice();
			if (this->Interactor)
			{
				this->ReleaseFocus();
			}
			break;
		}
		// Call parent to handle all other states and perform additional work
		this->Superclass::OnMiddleButtonUp();
		break;
	default:
		break;
	}
}

void myVtkInteractorStyleImage::OnMiddleButtonDown()
{
}

void myVtkInteractorStyleImage::OnMiddleButtonUp()
{
}

void myVtkInteractorStyleImage::OnRightButtonDown()
{
}

void myVtkInteractorStyleImage::OnRightButtonUp()
{
}

vtkStandardNewMacro(myVtkInteractorStyleImage);