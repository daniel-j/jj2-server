#include "logger.h"

Logger::Logger(QObject *parent) : QObject(parent) {

}


void Logger::log(QString str) {
	QTextStream qout(stdout);
	qout << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " " << str << endl;
}
