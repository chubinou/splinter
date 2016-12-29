#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QQmlDebuggingEnabler>
#include "osd.h"
#include "splitmodel.h"
#include "runner.h"
#include "server.h"

int main(int argc, char *argv[])
{
	QQmlDebuggingEnabler enabler;

	QGuiApplication app(argc, argv);
	qmlRegisterType<OSD>("org.chub", 1, 0, "OSD");
	qmlRegisterType<SplitModel>("org.chub", 1, 0, "SplitModel");

	QQmlApplicationEngine engine;
	QQmlContext *ctxt = engine.rootContext();


	Runner runner("splits.json");
	QList<QObject*> splitmodel = runner.splitsList();

	Server server(runner);
	server.listen(QHostAddress::Any, 16834);

	ctxt->setContextProperty("runner", &runner);
	ctxt->setContextProperty("game", QVariant::fromValue(runner.title()));
	ctxt->setContextProperty("category", QVariant::fromValue(runner.category()));
	ctxt->setContextProperty("splits", QVariant::fromValue(splitmodel));

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	return app.exec();
}
