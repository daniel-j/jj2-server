#include "application.h"
#include <QDir>

Application::Application(QStringList args, QObject* parent) :
	QObject(parent),
	settings(QDir::currentPath()+"/"+DATA_DIR+"/"+SETTINGS_FILE, QSettings::IniFormat)
{
	this->args = args;
	this->server = new Server(this);
}

Application::~Application() {
	stopServer();
	delete this->server;
}

void Application::log(QVariant msg) {
	QString str = Logger::log(msg);
	emit logSignal(str);
}
void Application::logSlot(QVariant msg) {
	this->log(msg);
}
QVariant Application::config(QString key, QVariant def) {
	return settings.value(key, def);
}

void Application::quit() {
	emit quitSignal();
}

void Application::runCommand(QString cmd) {
	if (cmd.length() == 0) return;

	// Is it a command?
	if (cmd.at(0) == '/') {
		cmd = cmd.mid(1); // Remove the slash

		if (cmd == "quit") {
			quit();

		} else if (cmd == "serverstop") {
			stopServer();
		} else if (cmd == "serverstart") {
			startServer();

		} else if (cmd == "list") {
			this->server->list();
		} else if (cmd == "delist") {
			this->server->delist();
		} else if (cmd == "relist") {
			this->server->relist();

		} else {
			log("Unknown command "+cmd);
		}
	}
}

void Application::startServer() {
	log("App: Starting server");

	this->server->start();
}

void Application::stopServer() {
	log("App: Stopping server");
	this->server->stop();
}
