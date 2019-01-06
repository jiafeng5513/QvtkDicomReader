#include "QvtkDicomViewer.h"
#include <QtWidgets/QApplication>

#include <vtkAutoInit.h>
#include "CommonHelper.h"
#include "Startup.h"
#include <qmutex.h>
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
#include<QDateTime>
#include<QTextStream>
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	static QMutex mutex;
	mutex.lock();

	QString text;

	switch (type)
	{
	case QtDebugMsg:
		text = QString("Debug:");
		break;

	case QtWarningMsg:
		text = QString("Warning:");
		break;

	case QtCriticalMsg:
		text = QString("Critical:");
		break;

	case QtFatalMsg:
		text = QString("Fatal:");
	}

	//QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
	QString context_info = QString("Line:(%1)").arg(context.line);
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	QString current_date = QString("(%1)").arg(current_date_time);
	QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

	QFile file("log.txt");
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream text_stream(&file);
	text_stream << message << "\r\n";
	//text_stream << msg << "\r\n";
	file.flush();
	file.close();

	mutex.unlock();
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qInstallMessageHandler(outputMessage);//
	CommonHelper::setStyle(QStringLiteral(":/QvtkDicomViewer/Resources/theme_black.qss"));
	//Startup starter;
	QvtkDicomViewer w;
	//starter.show();
	w.show();//在里边show自己给启动画面留出时间
	//starter.hide();
	return a.exec();
}
