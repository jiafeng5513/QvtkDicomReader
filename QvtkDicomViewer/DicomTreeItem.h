#pragma once
/*
 * DicomTreeItem.cpp
 * DicomDir树的自定义Item
 * 与DicomDirTreeModel组成MV模式
 * 预计缩略图功能将使用这个方案实现
 */
#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>
#include <QStandardItem>


class DicomTreeItem:public QStandardItem
{
public:
	explicit DicomTreeItem(const QVector<QVariant> &data, DicomTreeItem *parent = 0);
	~DicomTreeItem();

	DicomTreeItem *child(int number);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	bool insertChildren(int position, int count, int columns);
	bool insertColumns(int position, int columns);
	DicomTreeItem *parent();
	bool removeChildren(int position, int count);
	bool removeColumns(int position, int columns);
	int childNumber() const;
	bool setData(int column, const QVariant &value);

private:
	QList<DicomTreeItem*> childItems;
	QVector<QVariant> itemData;
	DicomTreeItem *parentItem;
};


#endif // TREEITEM_H
