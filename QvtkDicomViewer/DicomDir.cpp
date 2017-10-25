#include "DicomDir.h"

#include <dcmdata/dcdicdir.h>
#include <dcmdata/dcdeftag.h>
#include <QTableWidget>
#include <QDebug>
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
	
	//std::filebuf fb;
	//fb.open("DicomDir.log", std::ios::out);//输出日志文件
	//std::ostream out(&fb);
	//dicomdir.print(out);
	//fb.close();
 // 
	//DcmFileFormat DCMFF = dicomdir.getDirFileFormat();
	//std::filebuf lb;
	//lb.open("DcmFileFormat.log", std::ios::out);//输出日志文件2
	//std::ostream out2(&lb);
	//DCMFF.print(out2);
	//lb.close();

	DicomDirRootRecord = &(dicomdir.getRootRecord());
	
	bool flag = DicomDirRootRecord->isEmpty();
	DcmDirectoryRecord *   PatientRecord = NULL;
	OFString tmpString;
	
	std::vector<PatientMsg*>*data=new std::vector<PatientMsg*>();//
	PatientMsg * temp_patient_msg;
	int row = 0;//行数

	while (((PatientRecord = DicomDirRootRecord->getSub(row)) != NULL))
	{
		temp_patient_msg=new PatientMsg();

		if (PatientRecord->findAndGetOFString(DCM_ReferencedFileID, tmpString).good())//DCM_PatientName
		{
			temp_patient_msg->FileID = (tmpString.empty() == true ? "空" : tmpString.c_str());
			qDebug() << "FileID : " << temp_patient_msg->FileID;
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientID, tmpString).good())
		{
			temp_patient_msg->PatientID = (tmpString.empty() == true ? "空" : tmpString.c_str());
			qDebug() << "PatientID:" << temp_patient_msg->PatientID;
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientName, tmpString).good())
		{
			temp_patient_msg->PatientName = (tmpString.empty() == true ? "空" : tmpString.c_str());
			qDebug() << "PatientName:" << temp_patient_msg->PatientName;
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientBirthDate, tmpString).good())
		{
			temp_patient_msg->BirthDate = (tmpString.empty() == true ? "空" : tmpString.c_str());
			qDebug() << "BirthDate:" << temp_patient_msg->BirthDate;
		}
		if (PatientRecord->findAndGetOFString(DCM_PatientSex, tmpString).good())
		{
			temp_patient_msg->Gender = (tmpString.empty() == true ? "空" : tmpString.c_str());
			qDebug() << "Gender:" << temp_patient_msg->Gender;
		}
		row = row + 1;
		data->push_back(temp_patient_msg);//压一个病人的数据
		//delete(temp_patient_msg);
	}

	for(int i=0;i<data->size();i++)
	{
		qDebug() << QStringLiteral("第") << i << QStringLiteral("条数据:");
		qDebug() << "FileID : " << (*data)[i]->FileID;
		qDebug() << "PatientID : " << (*data)[i]->PatientID;
		qDebug() << "PatientName : " << (*data)[i]->PatientName;
		qDebug() << "BirthDate : " << (*data)[i]->BirthDate;
		qDebug() << "Gender : " << (*data)[i]->Gender;
	}
	if (row<=0)
	{
		//空,函数退出
	}else
	{
		//正常,构造表格
		ui.tableWidget->setColumnCount(5);
		ui.tableWidget->setRowCount(row);//行数
		ui.tableWidget->setHorizontalHeaderLabels(
			QStringList() << "File ID" << "Patient ID" << "Patient Name" << "Birth Date" << "Gender");
		ui.tableWidget->verticalHeader()->setVisible(false); // 隐藏水平header
		//ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);   // 单个选中
		ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
		//ui.tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  // 可以选中多个 
		ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
		/* 设置 Id Name 字段的值不能修改 */
		/* 设置不可修改的Id Name两列的背景颜色为灰色 */
		for (int i = 0; i < 5; i++) {
			QTableWidgetItem *item = new QTableWidgetItem();
			item->setBackground(QBrush(QColor(Qt::lightGray)));
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 0, item);
		}
		for (int i = 0; i < 5; i++) {
			QTableWidgetItem *item = new QTableWidgetItem();
			item->setBackground(QBrush(QColor(Qt::lightGray)));
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 1, item);
		}
		//填充数据开始
		for (int i = 0; i < ui.tableWidget->rowCount(); i++) {
			QTableWidgetItem *item_1 = new QTableWidgetItem();
			item_1->setText((*data)[i]->FileID);
			ui.tableWidget->setItem(i, 0, item_1);

			QTableWidgetItem *item_2 = new QTableWidgetItem();
			item_2->setText((*data)[i]->PatientID);
			ui.tableWidget->setItem(i, 1, item_2);
		}

		for (int i = 0; i < ui.tableWidget->rowCount(); i++) {
			QTableWidgetItem *item_max = new QTableWidgetItem();
			item_max->setText((*data)[i]->PatientName);
			item_max->setBackground(QBrush(QColor(Qt::lightGray)));
			item_max->setFlags(item_max->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 2, item_max);

			QTableWidgetItem *item_min = new QTableWidgetItem();
			item_min->setText((*data)[i]->BirthDate);
			item_min->setBackground(QBrush(QColor(Qt::lightGray)));
			item_min->setFlags(item_min->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 3, item_min);

			QTableWidgetItem *item_type = new QTableWidgetItem();
			item_type->setText((*data)[i]->Gender);
			item_type->setBackground(QBrush(QColor(Qt::lightGray)));
			item_type->setFlags(item_type->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 4, item_type);
		}
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
