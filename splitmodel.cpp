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
	m_splitState(SplitState::SEGMENT_NOT_STARTED)
{
}

QString SplitModel::bestSegment()
{
	return formatTimeFromMS(m_bestSegment);
}

QString SplitModel::bestCumul()
{
	return formatTimeFromMS(m_bestCumul);
}

QString SplitModel::current()
{
	return formatTimeFromMS(m_current);
}

QString SplitModel::currentDiffBest()
{
	return formatDiffTimeFromMS(m_current - m_bestSegment);
}

SplitModel::SplitState SplitModel::state()
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
	if (m_current < m_bestSegment) {
		m_splitState = SplitState::SEGMENT_DONE_BETTER;
	} else {
		m_splitState = SplitState::SEGMENT_DONE_WORSE;
	}
	emit stateChanged(m_splitState);
	emit currentChanged(formatTimeFromMS(m_current));
	emit currentDiffBestChanged(formatDiffTimeFromMS(m_current - m_bestSegment));
}

