#pragma once
#include "vtkCommand.h"
#include <vtkBiDimensionalWidget.h>
#include <vtkBiDimensionalRepresentation2D.h>
class vtkBiDimensionalCallback : public vtkCommand
{
public:
	static vtkBiDimensionalCallback* New();

	virtual void Execute(vtkObject* caller, unsigned long, void*);
	vtkBiDimensionalCallback();
};

