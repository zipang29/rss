#ifndef H_MAINWINDOW
#define H_MAINWINDOW

#include <QtWidgets>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(QString index);

	public slots:
		void simpleQuery();

	private:
		Ui::MainWindow ui;
		QString index;
};

#endif
