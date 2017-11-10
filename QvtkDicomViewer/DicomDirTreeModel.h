#pragma once
#include <QStandardItem>

/*
 * 标准Model的子类,主要是为了兼容自定义的item
 */
class DicomDirTreeModel:public QStandardItemModel
{
public:
	DicomDirTreeModel();
	~DicomDirTreeModel();
	//DicomTreeItem

};

