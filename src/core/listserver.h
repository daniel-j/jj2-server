#ifndef LISTSERVER_H
#define LISTSERVER_H

#include <QObject>
#include <QTcpSocket>

class Listserver : public QObject {
	Q_OBJECT

	private:
		QTcpSocket* listserverSock; // Socket to the listserver

		QStringList listservers;
		unsigned short listPort;
		bool isListed;

		void connectList();
		void disconnectList();

	public:
		explicit Listserver(QObject *parent = 0);
		virtual ~Listserver();

		void start();
		void stop();

		void list();
		void delist();
		void relist();

	signals:
		void log(QVariant msg);
		QVariant config(QString key, QVariant def = 0);

	private slots:
		void listConnected();
		void listProcessPackets();
		void listDisconnected();
		void listError();

};

#endif // LISTSERVER_H

