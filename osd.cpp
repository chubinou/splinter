#include "osd.h"

OSD::OSD(QQuickItem* parent) :
	QQuickWindow(parent ? parent->window() : 0)
{
	setClearBeforeRendering(true);

	setColor(QColor(Qt::transparent));
	setFlags(Qt::FramelessWindowHint | Qt::Window | Qt::BypassWindowManagerHint);
}
