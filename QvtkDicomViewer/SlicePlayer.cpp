#include "SlicePlayer.h"
#include <QTimer>


SlicePlayer::SlicePlayer(int min, int max, int current, int timespan)
{
	_Min = min;
	_Max = max;
	_Current = current;
	_TimeSpan = timespan;
}

SlicePlayer::~SlicePlayer()
{
}

void SlicePlayer::run()
{
	while(true)
	{
		pause.lock();
		if(_Current>=_Min&&_Current<=_Max)
		{
			_Current++;
			emit isTimeToTurnNextSlice();
		}
		else
		{
			_Current = _Min;
			emit isTimeToReset();
		}
		//QTimer::singleShot(_TimeSpan, &eventloop, SLOT(quit())); // µÈ´ý_TimeSpanºÁÃë
		msleep(_TimeSpan);
		pause.unlock();
	}
}

void SlicePlayer::setCurrentSlice(int current)
{
	_Current = current;
	
}
