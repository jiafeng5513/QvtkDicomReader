#include "DicomDir.h"

#include <dcmdata/dcdicdir.h>
#include <dcmdata/dcdeftag.h>

/*
 *默认构造方法
 */
DicomDir::DicomDir(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}
/*
 *含参构造方法,传入DicomDIR文件的绝对路径
 */
DicomDir::DicomDir(QString DicomDirFilePath, QWidget* parent)
{
	ui.setupUi(this);

	//TODO:用户初始化代码区域

	/*=============方法全局变量区===============*/
	DcmDirectoryRecord * DicomDirRootRecord;//用于提取该文件的根目录
	/*==========================================*/

	DcmDicomDir dicomdir(DicomDirFilePath.toStdString().c_str());
	
	std::filebuf fb;
	fb.open("DicomDir.log", std::ios::out);//输出日志文件
	std::ostream out(&fb);
	dicomdir.print(out);
	fb.close();

	DcmFileFormat DCMFF = dicomdir.getDirFileFormat();
	std::filebuf lb;
	lb.open("DcmFileFormat.log", std::ios::out);//输出日志文件2
	std::ostream out2(&lb);
	DCMFF.print(out2);
	lb.close();

	DicomDirRootRecord = &(dicomdir.getRootRecord());
	
	bool flag = DicomDirRootRecord->isEmpty();
	DcmDirectoryRecord *   PatientRecord = NULL;
	OFString tmpString;

	int row = 0;//nextSub(PatientRecord)
	while (((PatientRecord = DicomDirRootRecord->getSub(row)) != NULL))
	{
		ui.tableWidget->setRowCount(row + 1);//添加一行
		if (PatientRecord->findAndGetOFString(DCM_ReferencedFileID, tmpString).good())//DCM_PatientName
		{
			ui.tableWidget->setItem(row, 0, new QTableWidgetItem(tmpString.c_str()));
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientID, tmpString).good())
		{
			ui.tableWidget->setItem(row, 1, new QTableWidgetItem(tmpString.c_str()));
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientBirthDate, tmpString).good())
		{
			ui.tableWidget->setItem(row, 2, new QTableWidgetItem(tmpString.c_str()));
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientSex, tmpString).good())
		{
			ui.tableWidget->setItem(row, 3, new QTableWidgetItem(tmpString.c_str()));
		}
		row = row + 1;
	}
}
/*
 * 析构函数
 */
DicomDir::~DicomDir()
{

}
/*
 * 点击确定按键
 */
void DicomDir::OnPushOk()
{
	int row1 = ui.tableWidget->currentItem()->row();//当前选中行
	emit sendData(ui.tableWidget->item(row1, 1)->text());//将病人的ID返回一界面在一界面进行病人的查询
	//下面的这段代码意义不明
	/*===================这里用到的变量===================*/
	QString data1, data2, data3, data4, data5;//意义不明的五个变量
	int row;//这个函数默认row中保存的是表格的行数

	/*if (ui.tableWidget1->item(0, 0) != 0) {
		data1 = ui.tableWidget1->item(0, 0)->text();
		for (int j = 1; j < row; j++) {
			data2 = ui.tableWidget->item(j, 0)->text();
			if (data1 == data2) {
				emit sendData(ui.tableWidget->item(j, 1)->text());
			}
		}
	}
	if (ui.tableWidget1->item(0, 1) != 0) {
		data1 = ui.tableWidget1->item(0, 1)->text();
		for (int j = 1; j < row; j++) {
			data2 = ui.tableWidget->item(j, 1)->text();
			if (data1 == data2) {
				emit sendData(ui.tableWidget->item(j, 1)->text());
			}
		}
	}
	if (ui.tableWidget1->item(0, 2) != 0) {
		data1 = ui.tableWidget1->item(0, 2)->text();
		for (int j = 1; j < row; j++) {
			data2 = ui.tableWidget->item(j, 2)->text();
			if (data1 == data2) {
				emit sendData(ui.tableWidget->item(j, 1)->text());
			}
		}
	}
	if (ui.tableWidget1->item(0, 3) != 0) {
		data1 = ui.tableWidget1->item(0, 3)->text();
		for (int j = 1; j < row; j++) {
			data2 = ui.tableWidget->item(j, 3)->text();
			if (data1 == data2) {
				emit sendData(ui.tableWidget->item(j, 1)->text());
			}
		}
	}*/
	this->close();
}
/*
 * 点击取消按键
 */
void DicomDir::OnPushCancel()
{
	this->close();
}
