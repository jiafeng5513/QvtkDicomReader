#pragma once

#include <QWidget>
#include "ui_Segmenter.h"
#include "vtkImageSegmentCallback.h"

/*
 * 实现分割功能
 * 
 * 初始化:传入一张图片的绝对路径
 * 
 */
class Segmenter : public QWidget
{
	Q_OBJECT

public:
	Segmenter(QWidget *parent = Q_NULLPTR);
	Segmenter(std::string Filename,QWidget *parent = Q_NULLPTR);
	~Segmenter();

private:
	Ui::Segmenter ui;
	std::string m_Filename;			//待分割处理的原图的绝对路径
	SegmentFunc m_segment_fun;		//用户选择的分割方法
signals:
	void SegmentFuncChange();		//用户重新选择/首次选择分割算法
public slots:
	void OnConnectedThreshold();	//联通阈值方法
	void OnWatershedThreshold();	//分水岭
	void OnNeighborhoodConnected(); //邻域连接法
	void OnConfidenceConnected();	//置信连接法
	void OnFastMarching();			//快速匹配
	void OnShapeDetection();		//形状检测

	void OnSegmentFuncChange();		//响应分割方法的变化/首次设置
};
