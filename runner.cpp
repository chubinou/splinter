#include "runner.h"
#include <QTime>
#include <QUrl>
#include <QDebug>
#include "utils.h"

Runner::Runner(QString filename):
	QObject(),
	m_timerPhase(TimerPhase::NotRunning),
	m_delta(0),
	m_best(INT64_MAX)
{
	load(filename);
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
		if (m_delta < m_best) {
			m_best = m_delta;
		}
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

QString Runner::title()
{
	return m_title;
}

QString Runner::category()
{
	return m_category;
}

void Runner::save()
{
	m_splitparser.setBest(m_best);
	m_splitparser.setSplits(m_splits);
	m_splitparser.save(m_filename);
}

void Runner::load(QString filename)
{
	m_filename = filename;
	m_splitparser.loadJsonObjectFromFile(m_filename);
	m_splits = m_splitparser.splits();
	m_title = m_splitparser.gameName();
	m_best = m_splitparser.best();
	m_category = m_splitparser.categoryName();
	reset();
	emit titleChanged();
	emit categoryChanged();
	emit splitsListChanged();
}

void Runner::load(QUrl url)
{
	QString filePath = url.toLocalFile();
	load(filePath);
}


int Runner::countSplitModelList(QQmlListProperty<SplitModel>* property)
{
	Runner *m = qobject_cast<Runner *>(property->object);
	return m->m_splits.size();

}

SplitModel* Runner::atSplitModelList(QQmlListProperty<SplitModel>* property, int index)
{
	Runner *m = qobject_cast<Runner *>(property->object);
	return m->m_splits.at(index);
}

QQmlListProperty<SplitModel> Runner::splitsList()
{
	return QQmlListProperty<SplitModel>(this, 0, &Runner::countSplitModelList, &Runner::atSplitModelList);
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
