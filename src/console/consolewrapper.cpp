#include <QCoreApplication>
#include "consolewrapper.h"

ConsoleWrapper::ConsoleWrapper(QObject* parent) :
	QObject(parent),
	qout(stdout),
	qin(stdin)
{

	app = new Application(QCoreApplication::arguments(), this);
	connect(app, SIGNAL(logSignal(QString)), this, SLOT(log(QString)));
	connect(app, SIGNAL(quitSignal()), this, SLOT(quitSlot()));

	qin_notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
	connect(qin_notifier, SIGNAL(activated(int)), this, SLOT(gotCommand()));

	app->startServer();
}

ConsoleWrapper::~ConsoleWrapper() {
	disconnect(qin_notifier, SIGNAL(activated(int)), this, SLOT(gotCommand()));
	delete qin_notifier;
	delete app;
}

void ConsoleWrapper::log(QString msg) {
	qout << msg << endl;
}

void ConsoleWrapper::gotCommand() {
	QString str = qin.readLine();

	app->runCommand(str);
}

void ConsoleWrapper::quitSlot() {
	QCoreApplication::quit();
}
