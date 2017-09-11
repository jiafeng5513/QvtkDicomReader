#pragma once

#include <QThread>
#include <QMutex>

class SlicePlayer :public QThread
{
	Q_OBJECT
public:
	SlicePlayer(int min,int max,int current,int timespan);
	~SlicePlayer();
	void run() override;
	void setCurrentSlice(int current);
private:
	int _Min;
	int _Max;
	int _Current;
	int _TimeSpan;
public:
	QMutex pause;//锁信号量
signals:
	void isTimeToTurnNextSlice();//是时候放下一张了
	void isTimeToReset();//是时候重置了
};
