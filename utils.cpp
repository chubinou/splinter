#include "utils.h"
#include <QTime>

namespace {
	static const qint64 one_min = 60 * 1000;
	static const qint64 one_hour = one_min * 60;
}

QString formatTimeFromMS(qint64 msec)
{
	QTime ts = QTime::fromMSecsSinceStartOfDay(msec);
	QString timeStr;
	if (msec < one_min) {
		timeStr = ts.toString("ss.zzz");
	} else if (msec < one_hour) {
		timeStr =  ts.toString("mm:ss.zzz");
	} else {
		timeStr = ts.toString("hh:mm:ss.zzz");
	}
	timeStr.chop(1);
	return timeStr;
}

QString formatDiffTimeFromMS(qint64 msec)
{
	QString timeStr = formatTimeFromMS(qAbs(msec));
	if (msec < 0) {
		timeStr.prepend('-');
	} else {
		timeStr.prepend('+');
	}
	timeStr.chop(1);
	return timeStr;
}
