#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QTcpServer>
#include <QTcpSocket>


class Server : public QObject {

	Q_OBJECT

	private:
		QTcpServer* tcpServer;
		QList<QTcpSocket *> tcpClientsList;
		QTcpSocket* listserverSock; // Socket to the listserver
		unsigned short port;

	public:
		Server();
		virtual ~Server();

		void setPort(int port);

		void start();
		void stop();

	private slots:
		void tcpConnectClient();
		void tcpProcessPackets();
		void tcpDisconnectClient();


};

#endif // SERVERMANAGER_H

