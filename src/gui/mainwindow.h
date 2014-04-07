#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core/application.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

	private:
		Ui::MainWindow *ui;
		Application* app;


	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void log(QString msg);
		void quitSlot();
};

#endif // MAINWINDOW_H
