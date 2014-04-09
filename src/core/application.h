#ifndef APPLICATION_H
#define APPLICATION_H

#define DATA_DIR "data"
#define SETTINGS_FILE "settings.ini"

#include <QSettings>

#include "logger.h"
#include "server.h"


class Application : public QObject {

	Q_OBJECT

	private:
		Server* server;

		QStringList args;
		QSettings settings;
		//ConsoleCommands* cc;

	public:
		explicit Application(QStringList args, QObject *parent = 0);
		virtual ~Application();

		void startServer();
		void log(QVariant msg);

		void runCommand(QString cmd);

		void quit();

	signals:
		void logSignal(QString msg);
		void quitSignal();

	public slots:
		void stopServer();
		void logSlot(QVariant msg);
		QVariant config(QString key, QVariant def = 0);

};

#endif // APPLICATION_H
