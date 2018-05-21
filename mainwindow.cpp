#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->pushButton_play->setEnabled(false);

	mConvertingProcess = new QProcess(this);
	mPlayingProcess = new QProcess(this);

	connect(mConvertingProcess, SIGNAL(started()), this, SLOT(processStarted()));
	connect(mConvertingProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
	connect(mConvertingProcess, SIGNAL(finished(int)), this, SLOT(convertingFinished()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_load_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load video"), "~/", tr("Videos (*.avi *.mkv *.mp4)"));
	if (!fileName.isEmpty())
	{
		ui->lineEdit_in->setText(fileName);
	}
}

void MainWindow::on_pushButton_save_clicked()
{
	QString fileNameIn = ui->lineEdit_in->text();
	QFileInfo fileInfo(fileNameIn);
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save video"), fileInfo.absolutePath(), tr("Videos (*.avi *.mkv *.mp4)"));
	if (!fileName.isEmpty())
	{
		ui->lineEdit_out->setText(fileName);
	}
}

void MainWindow::on_pushButton_convert_clicked()
{
	QString program = "ffmpeg";
	QStringList arguments;
	QString input = ui->lineEdit_in->text();
	if(input.isEmpty())
	{
		qDebug() << "No input";
		QMessageBox::information(this, tr("ffmpeg"), tr("Input file not specified"));
		return;
	}
	QString output = ui->lineEdit_out->text();
	if(output.isEmpty())
	{
		qDebug() << "No output";
		QMessageBox::information(this, tr("ffmpeg"), tr("Output file not specified"));
		return;
	}
	QString fileName = ui->lineEdit_out->text();
	QFileInfo fileInfo(fileName);
	qDebug() << "output file chceck" << fileName;
	qDebug() << "QFile::exists(fileName) = " << QFile::exists(fileName);
	if (QFile::exists(fileName))
	{
		if (QMessageBox::question(this, tr("ffmpeg"),
															tr("There already exists a file called \n%1 \nin the current directory. \nOverwrite?").arg(fileInfo.fileName()),
															QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
			return;
		QFile::remove(fileName);
		while (QFile::exists(fileName))
		{
			qDebug() << "output file still there";
		}
	}

	//ffmpeg -i inputFile.mkv -codec copy outputFile.mp4
	arguments << "-i" << input << "-codec" << "copy" << output;

	qDebug() << arguments;

	mConvertingProcess->setProcessChannelMode(QProcess::MergedChannels);
	mConvertingProcess->start(program, arguments);
}

void MainWindow::on_pushButton_play_clicked()
{
	QString program = "xdg-open";
	QStringList arguments;
	QString output = ui->lineEdit_out->text();
	arguments << output;
	mPlayingProcess->start(program, arguments);
}

void MainWindow::processStarted()
{
	qDebug() << "processStarted()";
}

void MainWindow::readyReadStandardOutput()
{
	mOutputString.append(mConvertingProcess->readAllStandardOutput());
	ui->textEdit->setText(mOutputString);
	ui->textEdit->verticalScrollBar()->setSliderPosition(ui->textEdit->verticalScrollBar()->maximum());
}

void MainWindow::convertingFinished()
{
	QString fileName = ui->lineEdit_out->text();
	if(QFile::exists(fileName))
	{
		ui->label_status->setText("Stav: Hotovo!");
		ui->pushButton_play->setEnabled(true);
	}
	else
	{
		ui->label_status->setText("Stav: Chyba!");
	}
}

void MainWindow::on_actionAbout_triggered()
{
	AboutDialog aboutDialog;
	aboutDialog.setModal(true);
	aboutDialog.exec();
}
