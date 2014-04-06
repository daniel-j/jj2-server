#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCoreApplication>
#include <QSettings>

#include "server.h"

class Application : public QObject {

	Q_OBJECT

	private:
		Server* server;
		QStringList args;
		QSettings settings;
	public:
		Application(QStringList args);
		virtual ~Application();

		void startServer();
		void stopServer();

		//void logStatusMessage(QVariant msg);

	signals:

	public slots:

};

#endif // APPLICATION_H
