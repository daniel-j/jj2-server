#ifndef LOGGER_H
#define LOGGER_H

#include <QTextStream>
#include <QDateTime>

class Logger {

	public:
		static QString log(QVariant msg);

};

#endif // LOGGER_H
