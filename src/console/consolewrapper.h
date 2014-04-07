#ifndef CONSOLEWRAPPER_H
#define CONSOLEWRAPPER_H

#include <QTextStream>
#include <QSocketNotifier>
#include "core/application.h"

class ConsoleWrapper : public QObject {
	Q_OBJECT

	private:
		Application* app;
		QTextStream qout;
		QTextStream qin;
		QSocketNotifier* qin_notifier;

	public:
		explicit ConsoleWrapper(QObject *parent = 0);
		virtual ~ConsoleWrapper();


	private slots:
		void log(QString msg);
		void gotCommand();
		void quitSlot();
};

#endif // CONSOLEWRAPPER_H
