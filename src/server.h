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
		explicit Server(QObject *parent = 0);
		virtual ~Server();

		void setPort(int port);

		bool start();
		void stop();

	private slots:
		void tcpConnectClient();
		void tcpProcessPackets();
		void tcpDisconnectClient();


};

#endif // SERVERMANAGER_H

