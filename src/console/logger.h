#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QTextStream>
#include <QDateTime>

class Logger : public QObject {

	Q_OBJECT

	public:
		explicit Logger(QObject *parent = 0);

	public slots:
		void log(QString str);

};

#endif // LOGGER_H
