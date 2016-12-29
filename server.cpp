#include <server.h>
#include <QRegularExpression>
#include <QMetaEnum>

Server::Server(Runner& runner, QObject* parent):
	QTcpServer(parent),
	m_runner(runner)
{
	connect(this, &QTcpServer::newConnection, this, &Server::onConnection);
}

void Server::onConnection()
{
	m_socket = nextPendingConnection();
	connect(m_socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
}



void Server::onReadyRead()
{
	while (m_socket->canReadLine())
	{
		QByteArray data = m_socket->readLine();
		QString line = QString::fromUtf8(data);
		QStringList list = line.split(QRegularExpression("[ \t\r\n]"), QString::SplitBehavior::SkipEmptyParts);
		if (list.empty()) {
			continue;
		}
		qInfo() << "query:" << line;
		QString cmd = list[0];
		if (cmd == "starttimer") {
			m_runner.starttimer();
		} else if (cmd == "startorsplit") {
			m_runner.startorsplit();
		} else if (cmd == "split") {
			m_runner.split();
		} else if (cmd == "unsplit") {
			m_runner.unsplit();
		} else if (cmd == "skipsplit") {
			m_runner.skipsplit();
		} else if (cmd == "pause") {
			m_runner.pause();
		} else if (cmd == "resume") {
			m_runner.resume();
		} else if (cmd == "reset") {
			m_runner.reset();
		} else if (cmd == "pausegametime") {
			m_runner.pausegametime();
		} else if (cmd == "unpausegametime") {
			m_runner.unpausegametime();
		} else if (cmd == "getcurrenttime") {
			qint64 currentTime = m_runner.elapsed_ms();
			reply(QString::number(currentTime));
		} else if (cmd == "getcurrenttimerphase") {
			QMetaEnum metaEnum = QMetaEnum::fromType<Runner::TimerPhase>();
			QString phase = metaEnum.valueToKey(m_runner.timerPhase());
			reply(phase);
		} else {
			qWarning() << "got unknowned command:" << line;
		}
	}
}

void Server::reply(QString msg)
{
	qInfo() << "reply" << msg;
	m_socket->write(msg.toUtf8());
	m_socket->write("\r\n");
	m_socket->flush();
}


