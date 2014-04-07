#include <QCoreApplication>
#include "mainwidget.h"
#include "application.h"


int main(int argc, char *argv[]) {
	QCoreApplication a(argc, argv);
	Application app(a.arguments());

	app.startServer();

	return a.exec();
}
