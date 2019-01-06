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
//从dirfile绝对路径初始化
/*
 * 注意,只有在这种情况下才需要显示界面并构造表格
 */
void DicomDir::InitDirExplorerFromDirPath(QString DicomDirFidlePath)
{
	m_database = DicomDataBase::getInstance();
	m_database->Init(DicomDirFidlePath.toStdString());
	ConstructsTable();
}
//从单图片文件的绝对路径初始化
void DicomDir::InitDirExplorerFromSingleFilePath(QString ImageFilePath)
{
	m_database = DicomDataBase::getInstance();
	m_database->InitFromSingleImage(ImageFilePath.toStdString());
	emit sendData(QString::fromStdString(m_database->PatientList.at(0)->PatientID));
	//获取PatientID并手动发信号
	//ConstructsTable();
}
//从series的文件夹路径初始化
void DicomDir::InitDirExplorerFromSeriesPath(QString SeriesPath)
{
	m_database = DicomDataBase::getInstance();
	m_database->InitFromSeriesFolder(SeriesPath.toStdString());
	emit sendData(QString::fromStdString(m_database->PatientList.at(0)->PatientID));
	//获取PatientID并手动发信号
	//ConstructsTable();
}
//从LIDC-IDRI文件夹路径初始化
void DicomDir::InitDirExplorerFromLIDCFolder(QString LIDCPath)
{
	m_database = DicomDataBase::getInstance();
	m_database->InitFromLIDC(LIDCPath.toStdString());
	ConstructsTable();
}

//析构函数
DicomDir::~DicomDir()
{

}

//构造表格
void DicomDir::ConstructsTable()
{
	/*
	*构造表格
	*/
	ui.tableWidget->clear();
	if (m_database->PatientList.size() == 0)
	{
		return;
	}
	else
	{
		//正常,构造表格
		ui.tableWidget->setColumnCount(4);
		ui.tableWidget->setRowCount(m_database->PatientList.size());//行数
		int size = m_database->PatientList.size();
		ui.tableWidget->setHorizontalHeaderLabels(
			QStringList() << "Patient ID" << "Patient Name" << "Birth Date" << "Gender");
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

//点击确定按键
void DicomDir::OnPushOk()
{
	/*
	 * 将当前选中行对应的病人的PatientID和DICOMDIR的绝对路径以字符串返回到接收点
	 */
	this->close();
	emit sendData(ui.tableWidget->item(ui.tableWidget->currentItem()->row(), 0)->text());
	
}

//点击取消按键
void DicomDir::OnPushCancel()
{
	this->close();
}
