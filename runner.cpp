#include "runner.h"
#include <QTime>
#include <QDebug>
#include "utils.h"

Runner::Runner(QList<SplitModel*> splits):
	QObject(),
	m_splits(splits),
	m_timerPhase(TimerPhase::NotRunning),
	m_delta(0)
{
	m_timer.start();
}

QString Runner::elapsed() const
{
	qint64 time_ms = elapsed_ms();
	return formatTimeFromMS(time_ms);
}

qint64 Runner::elapsed_ms() const
{
	qint64 time_ms;

	if (m_timerPhase == TimerPhase::Running) {
		time_ms = m_timer.elapsed();
	} else {
		time_ms = 0;
	}
	return time_ms + m_delta;
}

Runner::TimerPhase Runner::timerPhase() const
{
	return m_timerPhase;
}


void Runner::starttimer()
{
	m_timer.start();
	m_delta = 0;
	m_timerPhase = TimerPhase::Running;
	m_currentSplit = m_splits.begin();
	(*m_currentSplit)->splitStart(0);
}

void Runner::startorsplit()
{
	if (m_timerPhase == TimerPhase::Running) {
		split();
	} else {
		starttimer();
	}
}

void Runner::split()
{
	if (m_currentSplit == m_splits.end()) {
		return;
	}
	qint64 elapsed = elapsed_ms();
	(*m_currentSplit)->splitDone(elapsed);
	m_currentSplit++;
	if (m_currentSplit == m_splits.end()) {
		m_delta = elapsed;
		m_timerPhase = TimerPhase::Ended;
		return;
	}
	(*m_currentSplit)->splitStart(elapsed);
}

void Runner::unsplit()
{
}

void Runner::skipsplit()
{

}

void Runner::reset()
{
	m_timer.restart();
	m_delta = 0;
	m_timerPhase = TimerPhase::NotRunning;
}

void Runner::initgametime()
{

}

void Runner::pausegametime()
{

}

void Runner::unpausegametime()
{

}

QList<QObject*> Runner::splitsList()
{
	QList<QObject*>	list;
	std::copy(m_splits.begin(), m_splits.end(), std::back_inserter(list));
	return list;
}

void Runner::pause()
{
	m_delta += m_timer.elapsed();
	m_timerPhase = TimerPhase::Paused;
}

void Runner::resume()
{
	m_timer.start();
	m_timerPhase = TimerPhase::Running;
}
