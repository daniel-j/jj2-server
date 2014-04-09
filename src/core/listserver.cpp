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
	connect(this->listserverSock, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(listError()));

	// old code, keep for reference
	/*PACKET_SERVERLIST_LIST packet;
	packet.port = 10052;
	packet.servername = "AAAAAA";
	packet.flags.isPrivate = true;
	packet.flags.gametype = 3;

	QByteArray bytes;
	QDataStream stream(&bytes, QIODevice::ReadWrite);
	stream << packet.port;
	//bytes.append(packet.servername);
	//bytes.append(QByteArray(33-packet.servername.length(), 0)); // padding
	stream << packet.flags;

	qDebug() << bytes.toHex();*/



}

Listserver::~Listserver() {
	this->stop();
	delete this->listserverSock;
}

void Listserver::connectList() {
	log("LIST: Connecting to "+this->listservers[0]+"...");
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

	PACKET_SERVERLIST_LIST packet;
	packet.port = 10052;
	QString servername = config("server/name").toStringList().join(",");
	packet.players = 1;
	packet.capacity = 1;
	packet.isPrivate = true;
	packet.gametype = 5;
	packet.unknownFlag = 0;
	QString version = "21  ";

	memset(packet.servername, 0, sizeof(packet.servername));
	QByteArray buf = servername.toLocal8Bit();
	memcpy(packet.servername, buf.data(), buf.size());

	memset(packet.version, 32, sizeof(packet.version));
	memcpy(packet.version, version.toLocal8Bit().data(), sizeof(packet.version));

	char* buffer = (char*)&packet;
	this->listserverSock->write(buffer, sizeof(PACKET_SERVERLIST_LIST));

	//qDebug() << QByteArray(buffer, sizeof(PACKET_SERVERLIST_LIST)).toHex();
}

void Listserver::listProcessPackets() {
	log("LIST: Got data: "+listserverSock->readAll().toHex());
}

void Listserver::listDisconnected() {
	log("LIST: Disconnected");
}

void Listserver::listError() {
	QAbstractSocket::SocketError error = this->listserverSock->error();
	QAbstractSocket::SocketState state = this->listserverSock->state();

	qDebug() << "LIST: Connection state:" << state;
}
