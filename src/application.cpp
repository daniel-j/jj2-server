#include "application.h"

Application::Application(QStringList args) : settings(QCoreApplication::applicationDirPath()+"/"+DATA_DIR+"/"+SETTINGS_FILE, QSettings::IniFormat) {
	this->args = args;
	this->server = NULL;
	this->cc = new ConsoleCommands(this);


	connect(this->cc, SIGNAL(stop()), this, SLOT(quit()));
}

Application::~Application() {
	this->stopServer();
	delete this->cc;
}

void Application::quit() {
	QCoreApplication::quit();
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

