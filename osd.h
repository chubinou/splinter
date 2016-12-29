#ifndef SPLINTER_OSD_HPP
#define SPLINTER_OSD_HPP

#include <QQuickItem>
#include <QQuickWindow>

class OSD : public QQuickWindow
{
	Q_OBJECT
public:
	OSD(QQuickItem* parent = 0);

};

#endif // SPLINTER_OSD_HPP
