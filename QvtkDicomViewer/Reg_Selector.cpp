#include <Reg_Selector.h>
#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <DicomReg.h>
#include <DicomReg.h>
enum Reg_enum
{
	Reg_Null,
	Reg_test,
	Reg_2Dtransform,
	Reg_AffineTrans,
	Reg_Multi
};


Reg_Selector::Reg_Selector(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.lineEdit->setReadOnly(true);
	ui.lineEdit->setPlaceholderText("文件一路径");
	ui.lineEdit_3->setReadOnly(true);
	ui.lineEdit_3->setPlaceholderText("文件二路径");
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(createContentMenu()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(createContentMenu2()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(submit()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(cancel()));
}


void Reg_Selector::createContentMenu()
{
	QString fileName = QFileDialog::getOpenFileName(
		                 this,
		                 tr("Open File"),
		                  NULL,// ".",
		                tr("*.*"));
	ui.lineEdit->setText(fileName);
}
void Reg_Selector::createContentMenu2()
{
	QString fileName = QFileDialog::getOpenFileName(
		this,
		tr("Open File"),
		NULL,// ".",
		tr("*.*"));
	ui.lineEdit_3->setText(fileName);
}
void Reg_Selector::submit()
{
	QString str = ui.lineEdit->text();
	QString str2 = ui.lineEdit_3->text();
	QByteArray arr = str.toLatin1();

	char *chr = arr.data();

	QByteArray arr2 = str.toLatin1();


	char *chr2 = arr2.data();

	Reg_enum  reg_enum = (Reg_enum)this->reg_count;
	char * argv[] = {
		" ",
		chr,
		chr2,
		//"C://Users//bao//Desktop//DICOM//S427870//S10//I10",
		"..//registration//ImageRegistration1Output.dcm",
		"..//registration//ImageRegistration1DifferenceAfter.dcm",
		"..//registration//ImageRegistration1DifferenceBefore.dcm"
		};

	//    INPUTS:  {BrainProtonDensitySliceBorder20.png}
	//    INPUTS:  {BrainProtonDensitySliceR10X13Y17S12.png}
	//    OUTPUTS: {ImageRegistration7Output.png}
	//    OUTPUTS: {ImageRegistration7DifferenceBefore.png}
	//    OUTPUTS: {ImageRegistration7DifferenceAfter.png}
	//    ARGUMENTS:    1.0   1.0   0.0
	char * argv1[] =
	{
		" ",
		//"BrainProtonDensitySliceBorder20.png",
		//"BrainProtonDensitySliceR10X13Y17S12.png",
		chr,
		chr2,
		"..//registration//ImageRegistration7Output.dcm",
		"..//registration//ImageRegistration7DifferenceBefore.dcm",
		"..//registration//ImageRegistration7DifferenceAfter.dcm",
		"1.0",
		"1.0",
		"0,0"
	};

	//    INPUTS:  {BrainProtonDensitySliceBorder20.png}
	//    INPUTS:  {BrainProtonDensitySliceR10X13Y17.png}
	//    OUTPUTS: {ImageRegistration9Output.png}
	//    OUTPUTS: {ImageRegistration9DifferenceBefore.png}
	//    OUTPUTS: {ImageRegistration9DifferenceAfter.png}
	//    ARGUMENTS:    1.0 300
	char * argv2[] =
	{
		" ",
		chr,
		chr2,
		//"BrainT1SliceBorder20.png",
		//"BrainProtonDensitySliceShifted13x17y.png",
		"..//registration//ImageRegistration9Output.dcm",
		"..//registration//ImageRegistration9DifferenceBefore.dcm",
		"..//registration//ImageRegistration9DifferenceAfter.dcm",
		"1.0",
		"300"
	};
     //   INPUTS: {BrainT1SliceBorder20.png}
	//    INPUTS:  {BrainProtonDensitySliceShifted13x17y.png}
	//    OUTPUTS: {MultiResImageRegistration1Output.png}
	//    ARGUMENTS: 128
	//    OUTPUTS: {MultiResImageRegistration1CheckerboardBefore.png}
	//    OUTPUTS: {MultiResImageRegistration1CheckerboardAfter.png}
	char * argv3[] =
	{
		" ",
		chr,
		chr2,
		//"BrainT1SliceBorder20.png",
		//"BrainProtonDensitySliceShifted13x17y.png",
		"..//registration//MultiResImageRegistration1Output.dcm",
		"128",
		"..//registration//MultiResImageRegistration1CheckerboardBefore.dcm",
		"..//registration//MultiResImageRegistration1CheckerboardAfter.dcm"
	};
	switch (reg_enum)
	{
	case Reg_Null:
		break;

	case Reg_test:
		reg_test(100, argv, this->qvtk);
		break;

	case Reg_2Dtransform:
		reg_2Dtransform(100, argv1, this->qvtk);
		break;

	case Reg_AffineTrans:
		reg_AffineTrans(8, argv2, this->qvtk);
		break;

	case Reg_Multi:
		reg_Multi(100, argv3, this->qvtk);
		break;

	default:
		break;
	}
	this->close();
}
void Reg_Selector::SetQvtk(QVTKWidget* qvtk)
{
	this->qvtk = qvtk;
}
void Reg_Selector::cancel() 
{
	this->close();
}

void Reg_Selector::SetCount(int count)
{
	this->reg_count = count;
}