#include "application.h"
#include <QDir>

Application::Application(QStringList args, QObject* parent) :
	QObject(parent),
	settings(QDir::currentPath()+"/"+DATA_DIR+"/"+SETTINGS_FILE, QSettings::IniFormat)
{
	this->args = args;
	this->server = NULL;
}

Application::~Application() {
	this->stopServer();
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
	if (cmd == "quit") {
		quit();
	}
}

void Application::startServer() {
	log("App: Starting server");
	this->server = new Server(this);

	this->server->start();
}

void Application::stopServer() {
	if (this->server != NULL) {
		delete this->server;
		this->server = NULL;
	}
}
