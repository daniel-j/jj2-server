#ifndef APPLICATION_H
#define APPLICATION_H

#define DATA_DIR "../data"
#define SETTINGS_FILE "settings.ini"

#include <QCoreApplication>
#include <QSettings>

#include "server.h"
//#include "consolecommands.h"

class Application : public QObject {

	Q_OBJECT

	private:
		Server* server;

		QStringList args;
		QSettings settings;
		//ConsoleCommands* cc;

	public:
		Application(QStringList args);
		virtual ~Application();

		void startServer();
		//void logStatusMessage(QVariant msg);

	signals:
		void log(QString str);

	public slots:
		void stopServer();
		void quit();

};

#endif // APPLICATION_H
