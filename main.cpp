#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlDebuggingEnabler>
#include <QCommandLineParser>
#include "osd.h"
#include "splitmodel.h"
#include "runner.h"
#include "server.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QCoreApplication::setApplicationName("splinter");
	QCoreApplication::setApplicationVersion("1.0");

	QCommandLineParser parser;
	parser.setApplicationDescription("linux game splitter");
	parser.addHelpOption();
	parser.addVersionOption();

	QCommandLineOption configOption(
		QStringList() << "c" << "config",
		"load the config file <config>", "config");

	parser.addOption(configOption);
	parser.process(app);

	QString splitFile = parser.value(configOption);

	qmlRegisterType<OSD>("org.chub", 1, 0, "OSD");
	qmlRegisterType<SplitModel>("org.chub", 1, 0, "SplitModel");

	QQmlApplicationEngine engine;
	QQmlContext *ctxt = engine.rootContext();

	Runner runner(splitFile);

	Server server(runner);
	server.listen(QHostAddress::Any, 16834);

	ctxt->setContextProperty("runner", &runner);

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	return app.exec();
}
