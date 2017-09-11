#include "vtkBiDimensionalCallback.h"

vtkBiDimensionalCallback* vtkBiDimensionalCallback::New()
{
	return new vtkBiDimensionalCallback;
}

void vtkBiDimensionalCallback::Execute(vtkObject* caller, unsigned long, void*)
{
	vtkBiDimensionalWidget* biDimensionalWidget =
		reinterpret_cast<vtkBiDimensionalWidget*>(caller);

	vtkBiDimensionalRepresentation2D* representation =
		static_cast<vtkBiDimensionalRepresentation2D*>(biDimensionalWidget->GetRepresentation());

	double p1[3];
	representation->GetPoint1DisplayPosition(p1);
	double p2[3];
	representation->GetPoint1DisplayPosition(p2);
	double p3[3];
	representation->GetPoint1DisplayPosition(p3);
	double p4[3];
	representation->GetPoint1DisplayPosition(p4);

	// Display one of the points, just so we know it's working
	std::cout << "P1: " << p1[0] << " " << p1[1] << " " << p1[2] << std::endl;
}

vtkBiDimensionalCallback::vtkBiDimensionalCallback()
{
}
