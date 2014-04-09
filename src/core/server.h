#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

#include "listserver.h"

class Server : public QObject {

	Q_OBJECT

	private:
		QTcpServer* tcpServer;
		QList<QTcpSocket *> tcpClientsList;
		Listserver* listserver;

		QUdpSocket* udpSocket;
		unsigned short port;

	public:
		explicit Server(QObject *parent = 0);
		virtual ~Server();

		void setPort(int port);

		bool start();
		void stop();

	signals:
		void log(QVariant msg);
		QVariant config(QString key, QVariant def = 0);

	private slots:
		void tcpConnectClient();
		void tcpProcessPackets();
		void tcpDisconnectClient();

		void udpProcessPackets();


};

#endif // SERVERMANAGER_H

