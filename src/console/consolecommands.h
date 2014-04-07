#ifndef CONSOLECOMMANDS_H
#define CONSOLECOMMANDS_H

#include <QObject>
#include <QDebug>
#include <QSocketNotifier>
#include <QTextStream>

class ConsoleCommands : public QObject {

	Q_OBJECT

	private:
		QSocketNotifier* cin_notifier;
		QTextStream* qin;

	public:
		explicit ConsoleCommands(QObject *parent = 0);
		virtual ~ConsoleCommands();

	signals:
		void stop();

	private slots:
		void parseCommand();
};

#endif // CONSOLECOMMANDS_H
