#include "TreeItem.h"

/*
treeitem.cpp

A container for items of data supplied by the simple tree model.
*/

#include "treeitem.h"

#include <QStringList>

//! [0]
DicomTreeItem::DicomTreeItem(const QVector<QVariant> &data, DicomTreeItem *parent)
{
	parentItem = parent;
	itemData = data;
}
//! [0]

//! [1]
DicomTreeItem::~DicomTreeItem()
{
	qDeleteAll(childItems);
}
//! [1]

//! [2]
DicomTreeItem *DicomTreeItem::child(int number)
{
	return childItems.value(number);
}
//! [2]

//! [3]
int DicomTreeItem::childCount() const
{
	return childItems.count();
}
//! [3]

//! [4]
int DicomTreeItem::childNumber() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<DicomTreeItem*>(this));

	return 0;
}
//! [4]

//! [5]
int DicomTreeItem::columnCount() const
{
	return itemData.count();
}
//! [5]

//! [6]
QVariant DicomTreeItem::data(int column) const
{
	return itemData.value(column);
}
//! [6]

//! [7]
bool DicomTreeItem::insertChildren(int position, int count, int columns)
{
	if (position < 0 || position > childItems.size())
		return false;

	for (int row = 0; row < count; ++row) {
		QVector<QVariant> data(columns);
		DicomTreeItem *item = new DicomTreeItem(data, this);
		childItems.insert(position, item);
	}

	return true;
}
//! [7]

//! [8]
bool DicomTreeItem::insertColumns(int position, int columns)
{
	if (position < 0 || position > itemData.size())
		return false;

	for (int column = 0; column < columns; ++column)
		itemData.insert(position, QVariant());

	foreach(DicomTreeItem *child, childItems)
		child->insertColumns(position, columns);

	return true;
}
//! [8]

//! [9]
DicomTreeItem *DicomTreeItem::parent()
{
	return parentItem;
}
//! [9]

//! [10]
bool DicomTreeItem::removeChildren(int position, int count)
{
	if (position < 0 || position + count > childItems.size())
		return false;

	for (int row = 0; row < count; ++row)
		delete childItems.takeAt(position);

	return true;
}
//! [10]

bool DicomTreeItem::removeColumns(int position, int columns)
{
	if (position < 0 || position + columns > itemData.size())
		return false;

	for (int column = 0; column < columns; ++column)
		itemData.remove(position);

	foreach(DicomTreeItem *child, childItems)
		child->removeColumns(position, columns);

	return true;
}

//! [11]
bool DicomTreeItem::setData(int column, const QVariant &value)
{
	if (column < 0 || column >= itemData.size())
		return false;

	itemData[column] = value;
	return true;
}
//! [11]
