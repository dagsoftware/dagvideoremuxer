#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>
#include <QScrollBar>
#include <QTranslator>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void on_pushButton_load_clicked();

		void on_pushButton_save_clicked();

		void on_pushButton_convert_clicked();

		void on_pushButton_play_clicked();

		void readyReadStandardOutput();
		void processStarted();
		void convertingFinished();

		void on_actionAbout_triggered();

	private:
		Ui::MainWindow *ui;
		QProcess *mConvertingProcess;
		QProcess *mPlayingProcess;
		QString mOutputString;
};

#endif // MAINWINDOW_H
