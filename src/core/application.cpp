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
QVariant Application::config(QString key) {
	return settings.value(key);
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
	connect(this->server, SIGNAL(log(QVariant)), this, SLOT(logSlot(QVariant)));
	connect(this->server, SIGNAL(config(QString)), this, SLOT(config(QString)));
	this->server->setPort(settings.value("server/port").toInt());
	this->server->start();
}

void Application::stopServer() {
	if (this->server != NULL) {
		delete this->server;
		this->server = NULL;
	}
}
