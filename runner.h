#ifndef SPLINTER_RUNNER_HPP
#define SPLINTER_RUNNER_HPP

#include <QObject>
#include <QElapsedTimer>
#include "splitmodel.h"
#include "splitparser.h"

class Runner : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString title READ title NOTIFY titleChanged)
	Q_PROPERTY(QString category READ category NOTIFY categoryChanged)
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
	Runner(QString filename);

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

	QString title();
	QString category();

	void save();
	void load(QString filename);

	QList<QObject*> splitsList();

signals:
	void splitsListChanged();
	void titleChanged();
	void categoryChanged();

private:
	QString m_filename;
	QString m_title;
	QString m_category;

	SplitParser m_splitparser;
	QList<SplitModel*> m_splits;
	QList<SplitModel*>::iterator m_currentSplit;
	TimerPhase m_timerPhase;
	qint64 m_delta;
	qint64 m_best;
	QElapsedTimer m_timer;
};

#endif /* SPLINTER_RUNNER_HPP */
