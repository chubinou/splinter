#include "splitmodel.h"
#include "utils.h"
#include <qdebug.h>


SplitModel::SplitModel():
	m_splitState(SplitState::SEGMENT_NOT_STARTED)
{

}

SplitModel::SplitModel(QString title, qint64 best_ms, qint64 bestCumul_ms):
	m_title(title),
	m_bestSegment(best_ms),
	m_bestCumul(bestCumul_ms),
	m_currentDiff(0),
	m_splitState(SplitState::SEGMENT_NOT_STARTED)
{
}

QString SplitModel::title() const
{
	return m_title;
}

QString SplitModel::bestSegment() const
{
	return formatTimeFromMS(m_bestSegment);
}

qint64 SplitModel::bestSegment_ms() const
{
	return m_bestSegment;
}

QString SplitModel::bestCumul() const
{
	return formatTimeFromMS(m_bestCumul);
}

qint64 SplitModel::bestCumul_ms() const
{
	return m_bestCumul;
}

QString SplitModel::current() const
{
	return formatTimeFromMS(m_current);
}

qint64 SplitModel::current_ms() const
{
	return m_current;
}

QString SplitModel::currentDiffBest() const
{
	return formatDiffTimeFromMS(m_currentDiff);
}

SplitModel::SplitState SplitModel::state() const
{
	return m_splitState;
}

void SplitModel::setState(SplitState state)
{
	m_splitState = state;
	emit stateChanged(m_splitState);
}

void SplitModel::splitStart(qint64 time_ms)
{
	m_splitStart = time_ms;
	m_splitState = SplitState::SEGMENT_RUNNING;
	emit stateChanged(m_splitState);
}

void SplitModel::splitDone(qint64 time_ms)
{
	m_current = time_ms - m_splitStart;
	m_currentDiff = m_current - m_bestSegment;
	if (m_currentDiff) {
		m_bestSegment = m_current;
		m_splitState = SplitState::SEGMENT_DONE_BETTER;
	} else {
		m_splitState = SplitState::SEGMENT_DONE_WORSE;
	}
	emit stateChanged(m_splitState);
	emit currentChanged(formatTimeFromMS(m_current));
	emit currentDiffBestChanged(formatDiffTimeFromMS(m_currentDiff));
}

