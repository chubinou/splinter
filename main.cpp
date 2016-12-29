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

class Config
{
public:
	Config(QString fname)
	{
		loadJsonObjectFromFile(fname);
	}

	void loadJsonObjectFromFile(QString fname)
	{
		QFile sourceFile(fname);
		sourceFile.open(QFile::ReadOnly | QFile::Text);
		QString jsonText = sourceFile.readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonText.toUtf8());
		if (!jsonDoc.isObject()) {
			qWarning() << "invalid json document";
		}
		m_config = jsonDoc.object();
	}

	QString getString(const QJsonObject& obj, QString key, QString defaultValue) const
	{
		if (!obj.contains(key) || !obj[key].isString()) {
			return defaultValue;
		}
		return obj[key].toString();
	}

	int getInt(const QJsonObject& obj, QString key, int defaultValue) const
	{
		if (!obj.contains(key) || !obj[key].isDouble()) {
			return defaultValue;
		}
		return obj[key].toInt();
	}

	QList<SplitModel*> getSplits() const
	{
		QList<SplitModel*> dataList;
		if (!m_config["splits"].isArray()) {
			qWarning() << "json: can't find splits list";
			return dataList;
		}
		const QJsonArray splits = m_config["splits"].toArray();
		qint64 cumul = 0;
		for(const QJsonValue value : splits)
		{
			if (!value.isObject()) {
				qWarning() << "json: invalid split object";
				continue;
			}
			const QJsonObject obj = value.toObject();
			if (!obj.contains("title") || !obj["title"].isString()) {
				qWarning() << "json: split has no title";
			}
			QString title = getString(obj, "title", "");
			int time = getInt(obj, "best", 42000);
			cumul += time;
			dataList.append(new SplitModel(title, time, cumul));
		}
		return dataList;
	}

	QString gameName() const
	{
		if (!m_config.contains("game") || !m_config["game"].isString()) {
			return "";
		}
		return m_config["game"].toString();
	}

	QString categoryName() const
	{
		if (!m_config.contains("category") || !m_config["category"].isString()) {
			return "";
		}
		return m_config["category"].toString();
	}

private:
	QJsonObject m_config;
};

int main(int argc, char *argv[])
{
	QQmlDebuggingEnabler enabler;
	Config conf("splits.json");

	QGuiApplication app(argc, argv);
	qmlRegisterType<OSD>("org.chub", 1, 0, "OSD");
	qmlRegisterType<SplitModel>("org.chub", 1, 0, "SplitModel");

	QQmlApplicationEngine engine;
	QQmlContext *ctxt = engine.rootContext();

	ctxt->setContextProperty("game", QVariant::fromValue(conf.gameName()));
	ctxt->setContextProperty("category", QVariant::fromValue(conf.categoryName()));

	QList<SplitModel*> splitList = conf.getSplits();

	Runner runner(splitList);
	ctxt->setContextProperty("runner", &runner);
	QList<QObject*> splitmodel = runner.splitsList();
	ctxt->setContextProperty("splits", QVariant::fromValue(splitmodel));
	Server server(runner);
	server.listen(QHostAddress::Any, 16834);

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	return app.exec();
}
