#include "logger.h"


QString Logger::log(QVariant msg) {
	QStringList list;
	list << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " " << msg.toString();
	return list.join("");
}
