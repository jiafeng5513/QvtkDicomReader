#pragma once

#include <QWidget>
#include "ui_DicomDir.h"

class DicomDir : public QWidget
{
	Q_OBJECT

public:
	DicomDir(QWidget *parent = Q_NULLPTR);
	DicomDir(QString DicomDirFilePath,QWidget *parent = Q_NULLPTR);
	~DicomDir();

private:
	Ui::DicomDir ui;
signals:
	void sendData(QString);//信号,通过该信号发出病人的ID
public slots:
	void OnPushOk();
	void OnPushCancel();
};
