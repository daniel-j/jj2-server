#include "listserver.h"

Listserver::Listserver(QObject *parent) :
	QObject(parent)
{
	connect(this, SIGNAL(config(QString, QVariant)), this->parent(), SIGNAL(config(QString, QVariant)));
	connect(this, SIGNAL(log(QVariant)), this->parent(), SIGNAL(log(QVariant)));

	this->listserverSock = new QTcpSocket(this);
	this->timeout = config("listserver/timeout", 5).toInt();
	this->reconnectDelay = config("listserver/reconnectDelay", 10).toInt();
	this->listPort = config("listserver/port", 10054).toInt();
	this->listservers = config("listserver/hosts", QString("list.jazzjackrabbit.com,list1.digiex.net,list2.digiex.net").split(",")).toStringList();
	this->currentServer = 0;
	this->isListed = config("listserver/list", true).toBool();
	this->instantReconnect = false;
	this->unexpectedDisconnect = true;

	connect(this->listserverSock, SIGNAL(connected()), this, SLOT(listConnected()));
	connect(this->listserverSock, SIGNAL(readyRead()), this, SLOT(listProcessPackets()));
	connect(this->listserverSock, SIGNAL(disconnected()), this, SLOT(listDisconnected()));
	connect(this->listserverSock, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(listError()));

	this->timeoutTimer = new QTimer(this);
	this->timeoutTimer->setInterval(this->timeout*1000);
	this->timeoutTimer->setSingleShot(true);
	connect(this->timeoutTimer, SIGNAL(timeout()), this, SLOT(listTimeout()));

	this->reconnectTimer = new QTimer(this);
	this->reconnectTimer->setInterval(this->reconnectDelay*1000);
	this->reconnectTimer->setSingleShot(true);
	connect(this->reconnectTimer, SIGNAL(timeout()), this, SLOT(listReconnect()));

	// old code, keep for reference, will be useful sometime in the future
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
	delete this->timeoutTimer;
	delete this->reconnectTimer;
	delete this->listserverSock;
}


void Listserver::connectList() {
	if (!this->isListed) return;
	if (this->listserverSock->state() != QTcpSocket::UnconnectedState) {
		this->disconnectList();
	}
	log("LIST: Connecting to "+this->listservers[this->currentServer]+"...");
	this->timeoutTimer->stop();
	this->timeoutTimer->start();
	this->reconnectTimer->stop();
	this->unexpectedDisconnect = true;
	this->listserverSock->connectToHost(this->listservers[this->currentServer], this->listPort);
}

void Listserver::disconnectList() {
	if (this->listserverSock->state() != QTcpSocket::UnconnectedState) {
		log("LIST: Disconnecting...");
		this->timeoutTimer->stop();
		this->reconnectTimer->stop();
		this->unexpectedDisconnect = false;

		this->listserverSock->abort();
		this->listserverSock->disconnectFromHost();
	}
}

void Listserver::start() {
	if (this->isListed && this->listserverSock->state() == QTcpSocket::UnconnectedState) {
		log("LIST: Starting...");
		this->connectList();
	}
}

void Listserver::stop() {
	if (this->isListed && this->listserverSock->state() != QTcpSocket::UnconnectedState) {
		log("LIST: Stopping...");
		timeoutTimer->stop();
		this->disconnectList();
		this->listserverSock->close();
	}
}

void Listserver::list() {
	if (!this->isListed) {
		log("LIST: Listing server...");
		this->isListed = true;

		this->connectList();
	}
}

void Listserver::delist() {
	if (this->isListed) {
		log("LIST: Delisting server...");
		this->isListed = false;
		//this->currentServer = 0; // Reset so it starts at the beginning of the list

		this->disconnectList();
	}
}

void Listserver::relist() {
	if (this->isListed) {
		if (this->listserverSock->state() == QTcpSocket::UnconnectedState) {
			log("LIST: Relisting server...");
			this->connectList();
		} else if (this->listserverSock->state() != QTcpSocket::ConnectingState) {
			log("LIST: Relisting server...");
			this->instantReconnect = true;
			this->disconnectList();
		} else {
			log("LIST: A list attempt is in progress");
		}
	}
}

void Listserver::listConnected() {
	this->timeoutTimer->stop();
	log("LIST: Connected to "+this->listserverSock->peerName());

	this->sendListPacket();
}

void Listserver::listProcessPackets() {
	log("LIST: Got data: "+listserverSock->readAll().toHex());
}

void Listserver::listDisconnected() {
	log("LIST: Disconnected");
	timeoutTimer->stop();
	if (this->instantReconnect) {
		this->instantReconnect = false;
		QTimer::singleShot(0, this, SLOT(listReconnect()));

	} else if (this->unexpectedDisconnect) {
		qDebug() << "UNEXPECTED DISCONNECT!";

		this->reconnectTimer->start();
	}
}

void Listserver::listError() {
	QAbstractSocket::SocketError error = this->listserverSock->error();
	QAbstractSocket::SocketState state = this->listserverSock->state();

	qDebug() << "LIST: Connection state:" << state;

	/*if (this->instantReconnect && state == QTcpSocket::UnconnectedState) {
		this->instantReconnect = false;
		this->connectList();
	}*/
}
void Listserver::listTimeout() {
	log("LIST: Connection timed out");
	this->disconnectList();
	this->currentServer++;
	if (this->currentServer >= this->listservers.length()) {
		this->currentServer = 0;

	} else {
		QTimer::singleShot(0, this, SLOT(listReconnect()));
	}

}

void Listserver::listReconnect() {
	this->connectList();
}

void Listserver::sendListPacket() {
	PACKET_LISTSERVER_LIST packet;
	packet.port = config("server/port", 10052).toInt();
	QString servername = config("server/name").toStringList().join(",");
	packet.players = 1;
	packet.capacity = 1;
	packet.isPrivate = true;
	packet.gametype = 5;
	packet.unknownFlag = 0;
	QString version = "21  ";

	QByteArray buf = servername.toLocal8Bit();
	memset(packet.servername, 0, sizeof(packet.servername));
	memcpy(packet.servername, buf.data(), buf.size());

	memset(packet.version, 32, sizeof(packet.version));
	memcpy(packet.version, version.toLocal8Bit().data(), sizeof(packet.version));

	char* buffer = (char*)&packet;
	this->listserverSock->write(buffer, sizeof(PACKET_LISTSERVER_LIST));

	qDebug() << QByteArray(buffer, sizeof(PACKET_LISTSERVER_LIST)).toHex();
}



