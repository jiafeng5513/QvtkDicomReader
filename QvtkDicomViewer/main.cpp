#include "QvtkDicomViewer.h"
#include <QtWidgets/QApplication>

#include <vtkAutoInit.h>
#include "CommonHelper.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
#include<vtkSmartPointer.h>  
#include<vtkSphereSource.h>  
#include<vtkPolyDataMapper.h>  
#include<vtkActor.h>  
#include<vtkImageViewer.h>  
#include<vtkRenderWindowInteractor.h>  
#include<vtkInteractorStyleImage.h>  
#include<vtkJPEGReader.h>  
#include<vtkRenderer.h>  
#include<QVTKWidget.h> 

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CommonHelper::setStyle(QStringLiteral(":/QvtkDicomViewer/Resources/theme_black.qss"));
	QvtkDicomViewer w;
	w.show();//在里边show自己给启动画面留出时间
	return a.exec();
}
