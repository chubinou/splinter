#ifndef SPLINTER_SPLIT_MODEL_LIST_HPP
#define SPLINTER_SPLIT_MODEL_LIST_HPP

#include <QAbstractListModel>
#include "splitmodel.h"

class SplitModelList : public QAbstractListModel
{
	Q_OBJECT
public:

	explicit SplitModelList(QList<SplitModel*> list, QObject *parent = 0);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

public slots:
	// Extra function to get the thing easily from outside Repeaters.
	SplitModel* split(int idx);

private:
	QList<SplitModel*> m_list;
};

#endif /* SPLINTER_SPLIT_MODEL_LIST_HPP */
