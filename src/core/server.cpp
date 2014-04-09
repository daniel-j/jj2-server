#include "server.h"
#include "application.h"

// constructor, initialize stuff here
Server::Server(QObject* parent) :
	QObject(parent)
{
	connect(this, SIGNAL(log(QVariant)), this->parent(), SLOT(logSlot(QVariant)));
	connect(this, SIGNAL(config(QString, QVariant)), this->parent(), SLOT(config(QString, QVariant)));

	this->port = config("server/port").toInt();

	this->tcpServer = new QTcpServer(this);
	this->udpSocket = new QUdpSocket(this);
	connect(this->tcpServer, SIGNAL(newConnection()), this, SLOT(tcpConnectClient()));
	connect(this->udpSocket, SIGNAL(readyRead()), this, SLOT(udpProcessPackets()));

	this->listserver = new Listserver(this);



}

// destructor, stop the server and delete the pointers
Server::~Server() {
	this->stop();

	//disconnect(this->tcpServer, SIGNAL(newConnection()), this, SLOT(tcpConnectClient()));
	delete this->tcpServer;
	delete this->udpSocket;
	delete this->listserver;
}

void Server::setPort(int port) {
	this->port = (unsigned int) port;
}

bool Server::start() {

	log("TCP: Starting server on port "+QString().setNum(this->port)+" ...");
	if (!this->tcpServer->listen(QHostAddress::Any, this->port)) {
		log("TCP: Unable to start server");
		this->stop();
		return false;
	}

	log("UDP: Starting server on port "+QString().setNum(this->port)+" ...");
	if (!udpSocket->bind(this->port, QUdpSocket::ReuseAddressHint|QUdpSocket::DontShareAddress)) {
		log("UDP: Unable to start server");
		this->stop();
		return false;
	}

	this->listserver->start();

	/*{
		log("LIST: Could not connect to listserver");
		delete this->listserverSock;
		this->listserverSock = NULL;
	}*/

	return true;
}

void Server::stop() {
	if (this->tcpServer->isListening()) {
		log("TCP: Stopping server...");
		for (int i = 0; i < this->tcpClientsList.size(); i++) {
			// else the disconnect slot is called when closing server
			disconnect(this->tcpClientsList[i], SIGNAL(disconnected()), this, SLOT(tcpDisconnectClient()));
			this->tcpClientsList[i]->close();
		}
		tcpServer->close();
		this->tcpClientsList.clear();
	}

	if (this->udpSocket->state() == QUdpSocket::BoundState) {
		log("UDP: Stopping server..");
		this->udpSocket->close();
	}

	this->listserver->stop();
}

void Server::tcpConnectClient() {
	QTcpSocket *newClient = this->tcpServer->nextPendingConnection();
	this->tcpClientsList << newClient;

	connect(newClient, SIGNAL(readyRead()), this, SLOT(tcpProcessPackets()));
	connect(newClient, SIGNAL(disconnected()), this, SLOT(tcpDisconnectClient()));

	log("TCP: Client connected");
}

void Server::tcpProcessPackets() {
	// Find who's sending
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	if (socket == 0) {
		return;
	}

	log("TCP: Data: " + socket->readAll());
}

void Server::tcpDisconnectClient() {
	// Find who's disconnecting
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	if (socket == 0) {
		return;
	}

	log("TCP: Client disconnected");
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

		log("UDP: Packet id: "+QString().setNum(datagram[1]));
	}
}
