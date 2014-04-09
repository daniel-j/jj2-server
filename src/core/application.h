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

	public:
		explicit Application(QStringList args, QObject *parent = 0);
		virtual ~Application();

		void stopServer();
		void startServer();

		void log(QVariant msg);
		void runCommand(QString cmd);
		void quit();

	signals:
		void logSignal(QString msg);
		void quitSignal();

		void list();
		void delist();
		void relist();

	public slots:
		void logSlot(QVariant msg);
		QVariant config(QString key, QVariant def = 0);

};

#endif // APPLICATION_H
