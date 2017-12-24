#include "CommonHelper.h"



CommonHelper::CommonHelper()
{
}


CommonHelper::~CommonHelper()
{
}
/*
 * ¼ÓÔØÊôÐÔ±í
 */
void CommonHelper::setStyle(const QString& style)
{
	QFile qss(style);
	qss.open(QFile::ReadOnly);
	qApp->setStyleSheet(qss.readAll());
	qss.close();
}
