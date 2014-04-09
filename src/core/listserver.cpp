#include "listserver.h"

Listserver::Listserver(QObject *parent) :
	QObject(parent)
{
	connect(this, SIGNAL(config(QString, QVariant)), this->parent(), SIGNAL(config(QString, QVariant)));
	connect(this, SIGNAL(log(QVariant)), this->parent(), SIGNAL(log(QVariant)));

	this->listserverSock = new QTcpSocket(this);
	this->listPort = config("listserver/port", 10054).toInt();
	this->listservers = config("listserver/hosts", QString("list2.digiex.net,list1.digiex.net,list.jazzjackrabbit.com").split(",")).toStringList();
	this->isListed = config("listserver/list", true).toBool();

	connect(this->listserverSock, SIGNAL(connected()), this, SLOT(listConnected()));
	connect(this->listserverSock, SIGNAL(readyRead()), this, SLOT(listProcessPackets()));
	connect(this->listserverSock, SIGNAL(disconnected()), this, SLOT(listDisconnected()));
	connect(this->listserverSock, SIGNAL(error()), this, SLOT(listError()));


}

Listserver::~Listserver() {
	this->stop();
	delete this->listserverSock;
}

void Listserver::connectList() {
	log("LIST: Connecting...");
	this->listserverSock->connectToHost(this->listservers[0], this->listPort);
}

void Listserver::disconnectList() {
	log("LIST: Disconnecting...");
	this->listserverSock->close();
}

void Listserver::start() {
	if (this->isListed && this->listserverSock->state() == QTcpSocket::UnconnectedState) {
		this->connectList();
	}
}

void Listserver::stop() {
	if (this->isListed && this->listserverSock->state() != QTcpSocket::UnconnectedState) {
		this->disconnectList();
	}
}

void Listserver::listConnected() {
	log("LIST: Connected to "+this->listserverSock->peerName());
}

void Listserver::listProcessPackets() {
	log("LIST: Got data:"+listserverSock->readAll());
}

void Listserver::listDisconnected() {
	log("LIST: Disconnected");
}

void Listserver::listError() {
	log(this->listserverSock->errorString());
}
