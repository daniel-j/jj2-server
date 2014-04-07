#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	app = new Application(QApplication::arguments());
	connect(app, SIGNAL(logSignal(QString)), this, SLOT(log(QString)));
	connect(app, SIGNAL(quitSignal()), this, SLOT(quitSlot()));

	connect(ui->consoleInput, SIGNAL(returnPressed()), this, SLOT(gotCommand()));

	app->startServer();
}

MainWindow::~MainWindow() {
	delete app;
	delete ui;
}

void MainWindow::log(QString msg) {
	ui->consoleOutput->appendPlainText(msg);
	ui->consoleOutput->repaint();
}

void MainWindow::gotCommand() {
	QString str = ui->consoleInput->text();
	ui->consoleInput->clear();
	app->runCommand(str);
}

void MainWindow::quitSlot() {
	QApplication::quit();
}
