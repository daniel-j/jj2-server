#include "server.h"
#include "application.h"

Server::Server(QObject *parent) : QObject(parent) {
	this->port = 0;
	this->tcpServer = NULL;
	this->listserverSock = NULL;
}
Server::~Server() {
	this->stop();
}


void Server::setPort(int port) {
	this->port = (unsigned int) port;
}

bool Server::start() {
	this->tcpServer = new QTcpServer(this);
	qDebug() << "TCP: Starting server on port" << this->port << "...";
	if (!this->tcpServer->listen(QHostAddress::Any, this->port)) {
		qDebug() << "TCP: Unable to start server" << endl;
		this->stop();
		return false;
	}
	qDebug() << "UDP: Starting server on port" << this->port << "...";
	this->udpSocket = new QUdpSocket(this);
	if (!udpSocket->bind(this->port, QUdpSocket::ReuseAddressHint|QUdpSocket::DontShareAddress)) {
		qDebug() << "UDP: Unable to start server";
		this->stop();
		return false;
	}
	connect(this->tcpServer, SIGNAL(newConnection()), this, SLOT(tcpConnectClient()));
	connect(this->udpSocket, SIGNAL(readyRead()),this, SLOT(udpProcessPackets()));
	return true;
}

void Server::stop() {
	if (this->tcpServer != NULL) {
		qDebug() << "TCP: Stopping server..";
		tcpServer->close();
		for (int i = 0; i < this->tcpClientsList.size(); i++) {
			// else the disconnect slot is called when closing server
			disconnect(this->tcpClientsList[i], SIGNAL(disconnected()), this, SLOT(tcpDisconnectClient()));
			this->tcpClientsList[i]->close();
		}
		this->tcpClientsList.clear();

		//disconnect(this->tcpServer, SIGNAL(newConnection()), this, SLOT(tcpConnectClient()));

		delete this->tcpServer;
		this->tcpServer = NULL;
	}

	if (this->udpSocket != NULL) {
		qDebug() << "UDP: Stopping server..";
		this->udpSocket->close();
		delete this->udpSocket;
		this->udpSocket = NULL;
	}

	if (this->listserverSock != NULL) {
		qDebug() << "LIST: Disconnecting from listserver";
		this->listserverSock->close();
		delete this->listserverSock;
		this->listserverSock = NULL;
	}
}

void Server::tcpConnectClient() {
	QTcpSocket *newClient = this->tcpServer->nextPendingConnection();
	this->tcpClientsList << newClient;

	connect(newClient, SIGNAL(readyRead()), this, SLOT(tcpProcessPackets()));
	connect(newClient, SIGNAL(disconnected()), this, SLOT(tcpDisconnectClient()));

	qDebug() << "TCP: Client connected";
}

void Server::tcpProcessPackets() {
	// Find who's sending
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	if (socket == 0) {
		return;
	}

	qDebug() << "TCP: Data:" << socket->readAll();
}

void Server::tcpDisconnectClient() {
	// Find who's disconnecting
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	if (socket == 0) {
		return;
	}

	qDebug() << "TCP: Client disconnected";
	disconnect(socket);
	this->tcpClientsList.removeOne(socket);
}

void Server::udpProcessPackets() {

	while (this->udpSocket->hasPendingDatagrams()) {
		QHostAddress rAddr;
		quint16 rPort;
		QByteArray datagram;
		qint64 dataRead = 0;
		int datagramSize = udpSocket->pendingDatagramSize();
		datagram.resize(datagramSize);

		while (dataRead < datagram.size()) {
			qint64 readNow = this->udpSocket->readDatagram(datagram.data()+dataRead, datagramSize, &rAddr, &rPort);
			if (readNow != -1) {
				dataRead += readNow;
				if (datagramSize > (datagram.size() - dataRead))
					datagramSize = (datagram.size() - dataRead);

			} else {
				qWarning() << (QString("Socket error : ").arg(udpSocket->errorString()));
				return;
			}
		}

		qDebug() << "UDP: Packet id" << (int)datagram[1];
	}
}
