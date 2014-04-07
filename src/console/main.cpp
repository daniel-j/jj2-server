#include <QCoreApplication>
#include "console/consolewrapper.h"

int main(int argc, char *argv[]) {
	QCoreApplication a(argc, argv);
	ConsoleWrapper cw;

	return a.exec();
}

