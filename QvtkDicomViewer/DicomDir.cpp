#include "DicomDir.h"

#include <dcmdata/dcdicdir.h>
#include <dcmdata/dcdeftag.h>
#include <QTableWidget>
#include <QDebug>
#include<QFileSystemModel>

#include<iomanip>
#include<fstream>
#include "DicomDataBase.h"

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
	DicomDataBase* m_database =DicomDataBase::getInstance();
	m_database->Init(DicomDirFilePath.toStdString());
	/*
	 *构造表格
	 */
	if (m_database->PatientList.size()==0)
	{
		return;
	}else
	{
		//正常,构造表格
		ui.tableWidget->setColumnCount(4);
		ui.tableWidget->setRowCount(m_database->PatientList.size());//行数
		ui.tableWidget->setHorizontalHeaderLabels(
			QStringList() <<  "Patient ID" << "Patient Name" << "Birth Date" << "Gender");
		ui.tableWidget->verticalHeader()->setVisible(false); // 隐藏水平header
		//ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);   // 单个选中
		ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
		//ui.tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  // 可以选中多个 
		ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
		/* 设置 Id Name 字段的值不能修改 */
		/* 设置不可修改的Id Name两列的背景颜色为灰色 */
		for (int i = 0; i < 4; i++) {
			QTableWidgetItem *item = new QTableWidgetItem();
			item->setBackground(QBrush(QColor(Qt::lightGray)));
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 0, item);
		}
		for (int i = 0; i < 4; i++) {
			QTableWidgetItem *item = new QTableWidgetItem();
			item->setBackground(QBrush(QColor(Qt::lightGray)));
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 1, item);
		}
		//填充数据开始
		for (int i = 0; i < ui.tableWidget->rowCount(); i++) {
			QTableWidgetItem *item_2 = new QTableWidgetItem();
			item_2->setText(QString::fromStdString(m_database->PatientList[i]->PatientID));
			ui.tableWidget->setItem(i, 0, item_2);
		}

		for (int i = 0; i < ui.tableWidget->rowCount(); i++) {
			QTableWidgetItem *item_max = new QTableWidgetItem();
			item_max->setText(QString::fromStdString(m_database->PatientList[i]->PatientName));
			item_max->setBackground(QBrush(QColor(Qt::lightGray)));
			item_max->setFlags(item_max->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 1, item_max);

			QTableWidgetItem *item_min = new QTableWidgetItem();
			item_min->setText(QString::fromStdString(m_database->PatientList[i]->StudyList[0]->PatientBirthDate));
			item_min->setBackground(QBrush(QColor(Qt::lightGray)));
			item_min->setFlags(item_min->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 2, item_min);

			QTableWidgetItem *item_type = new QTableWidgetItem();
			item_type->setText(QString::fromStdString(m_database->PatientList[i]->StudyList[0]->PatientSex));
			item_type->setBackground(QBrush(QColor(Qt::lightGray)));
			item_type->setFlags(item_type->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(i, 3, item_type);
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
