#ifndef SPLINTER_SERVER_HPP
#define SPLINTER_SERVER_HPP

#include <QTcpServer>
#include <QTcpSocket>
#include "runner.h"


class Server : public QTcpServer
{
	Q_OBJECT

public:
	Server(Runner& runner, QObject* parent = nullptr);

private slots:
	void onConnection();
	void onReadyRead();

private:
	void reply(QString msg);
private:
	Runner& m_runner;
	QTcpSocket* m_socket;
};

#endif /* SPLINTER_SERVER_HPP */
