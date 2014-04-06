#include "server.h"
#include <QCoreApplication>

Server::Server() {
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

void Server::start() {
	this->tcpServer = new QTcpServer(this);
	qDebug() << "Starting TCP server on port" << this->port;
	if (!this->tcpServer->listen(QHostAddress::Any, this->port)) {
		qDebug() << "Starting TCP server failed!" << endl;
		return;
	}
	connect(this->tcpServer, SIGNAL(newConnection()), this, SLOT(tcpConnectClient()));
}

void Server::stop() {
	qDebug() << "Stopping server..";
	if (this->tcpServer != NULL) {
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

	QCoreApplication::quit();
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
	// Find who's disconnecting, if we can't, just give up
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	if (socket == 0) {
		return;
	}

	qDebug() << "socket disconnected";
	disconnect(socket);
	this->tcpClientsList.removeOne(socket);
}


