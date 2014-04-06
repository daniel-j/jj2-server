#include "application.h"

Application::Application(QStringList args) : settings(QCoreApplication::applicationDirPath()+"/settings.ini", QSettings::IniFormat) {
	this->args = args;
	this->server = NULL;
}

Application::~Application() {
	this->stopServer();
}

void Application::startServer() {
	this->server = new Server();
	this->server->setPort(settings.value("server/port").toInt());
	this->server->start();
}

void Application::stopServer() {
	if (this->server != NULL) {
		this->server->stop();
		delete this->server;
		this->server = NULL;
	}
}



/*void Application::logStatusMessage(QVariant msg) {
	qDebug() << msg << endl;
}*/

