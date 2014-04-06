#include "server.h"

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
	qDebug() << "Starting TCP server on port" << this->port;
	if (!this->tcpServer->listen(QHostAddress::Any, this->port)) {
		qDebug() << "Starting TCP server failed!" << endl;
		return false;
	}
	connect(this->tcpServer, SIGNAL(newConnection()), this, SLOT(tcpConnectClient()));
	return true;
}

void Server::stop() {
	if (this->tcpServer != NULL) {
		qDebug() << "Stopping server..";
		tcpServer->close();
		for (int i = 0; i < this->tcpClientsList.size(); i++) {
			this->tcpClientsList[i]->close();
			this->tcpClientsList[i]->deleteLater();
		}
		this->tcpClientsList.clear();

		disconnect(this->tcpServer, SIGNAL(newConnection()), this, SLOT(tcpConnectClient()));

		delete this->tcpServer;
		this->tcpServer = NULL;
	}
	if (this->listserverSock != NULL) {
		delete this->listserverSock;
		this->listserverSock = NULL;
	}
}

void Server::tcpConnectClient() {
	QTcpSocket *newClient = this->tcpServer->nextPendingConnection();
	this->tcpClientsList << newClient;

	connect(newClient, SIGNAL(readyRead()), this, SLOT(tcpProcessPackets()));
	connect(newClient, SIGNAL(disconnected()), this, SLOT(tcpDisconnectClient()));

	qDebug() << "client connected";
}

void Server::tcpProcessPackets() {
	// Find who's sending
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	if (socket == 0) {
		return;
	}

	qDebug() << "data: " << socket->readAll();
}

void Server::tcpDisconnectClient() {
	// Find who's disconnecting
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	if (socket == 0) {
		return;
	}

	qDebug() << "client disconnected";
	disconnect(socket);
	this->tcpClientsList.removeOne(socket);
}


