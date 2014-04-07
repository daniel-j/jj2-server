#include "consolecommands.h"

ConsoleCommands::ConsoleCommands(QObject *parent) : QObject(parent) {
	this->cin_notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
	this->qin = new QTextStream(stdin);

	connect(this->cin_notifier, SIGNAL(activated(int)), this, SLOT(parseCommand()));
}

ConsoleCommands::~ConsoleCommands() {
	disconnect(this->cin_notifier, SIGNAL(activated(int)), this, SLOT(parseCommand()));
	delete this->cin_notifier;
	delete this->qin;
}

void ConsoleCommands::parseCommand() {
	QString str = this->qin->readLine();

	//qDebug() << str;

	if (str == "stop") {
		emit stop();
	}
}

