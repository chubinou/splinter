#ifndef SPLINTER_SPLIT_MODEL_HPP
#define SPLINTER_SPLIT_MODEL_HPP

#include <QObject>
#include <QTime>

class SplitModel : public QObject
{
	Q_OBJECT

public:
	enum SplitState {
		SEGMENT_NOT_STARTED,
		SEGMENT_RUNNING,
		SEGMENT_DONE_BETTER,
		SEGMENT_DONE_WORSE
	};
	Q_ENUM(SplitState)

	Q_PROPERTY(QString title READ title NOTIFY titleChanged)
	Q_PROPERTY(QString bestSegment READ bestSegment NOTIFY bestSegmentChanged)
	Q_PROPERTY(QString bestCumul READ bestCumul NOTIFY bestCumulChanged)
	Q_PROPERTY(QString current READ current NOTIFY currentChanged)
	Q_PROPERTY(QString currentDiffBest READ currentDiffBest NOTIFY currentDiffBestChanged)
	Q_PROPERTY(SplitState splitstate READ state WRITE setState NOTIFY stateChanged)

public:
	SplitModel();
	SplitModel(QString title, qint64 best_ms, qint64 bestCumul_ms);

public:
	QString title() const;
	QString bestSegment() const;
	qint64 bestSegment_ms() const;
	QString bestCumul() const;
	qint64 bestCumul_ms() const;
	QString current() const;
	qint64 current_ms() const;
	QString currentDiffBest() const;
	SplitState state() const;

	void setState(SplitState state);
	void splitStart(qint64 time_ms);
	void splitDone(qint64 time_ms);

signals:
	void titleChanged(QString);
	void bestSegmentChanged(QString);
	void bestCumulChanged(QString);
	void currentChanged(QString);
	void stateChanged(SplitState);
	void currentDiffBestChanged(QString);

private:
	QString m_title;
	qint64 m_splitStart;
	qint64 m_bestSegment;
	qint64 m_bestCumul;
	qint64 m_current;
	qint64 m_currentDiff;
	SplitState m_splitState;
};

#endif /* SPLINTER_SPLIT_MODEL_HPP */
