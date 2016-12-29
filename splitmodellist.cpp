#include <splitmodellist.h>

SplitModelList::SplitModelList(QList<SplitModel*> list, QObject* parent):
	QAbstractListModel(parent),
	m_list(list)
{

}

int SplitModelList::rowCount(const QModelIndex& /*parent*/) const
{
	return m_list.size();
}

QVariant SplitModelList::data(const QModelIndex& index, int /*role*/) const
{
	int i = index.row();
	if (i < 0 || i >= m_list.size()) {
		return QVariant(QVariant::Invalid);
	}

	return QVariant::fromValue(m_list[i]);
}

SplitModel* SplitModelList::split(int idx)
{
	if (idx < 0 || idx >= m_list.size()) {
		return nullptr;
	}

	return m_list[idx];
}

