#ifndef SPLINTER_RUNNER_HPP
#define SPLINTER_RUNNER_HPP

#include <QObject>
#include <QElapsedTimer>
#include "splitmodel.h"
#include "splitmodellist.h"

class Runner : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QList<QObject*> splitsList READ splitsList NOTIFY splitsListChanged)

public:
	enum TimerPhase
	{
		NotRunning,
		Running,
		Ended,
		Paused
	};
	Q_ENUM(TimerPhase)

public:
	Runner(QList<SplitModel*> splits);

public slots:
	QString elapsed() const;
	qint64 elapsed_ms() const;
	TimerPhase timerPhase() const;

	void starttimer();
	void startorsplit();
	void split();
	void unsplit();
	void skipsplit();
	void pause();
	void resume();
	void reset();
	void initgametime();
	void pausegametime();
	void unpausegametime();

	QList<QObject*> splitsList();

signals:
	void splitsListChanged();

private:
	QList<SplitModel*> m_splits;
	QList<SplitModel*>::iterator m_currentSplit;
	TimerPhase m_timerPhase;
	qint64 m_delta;
	QElapsedTimer m_timer;
};

#endif /* SPLINTER_RUNNER_HPP */
