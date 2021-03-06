#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(QString("dagvideoremuxer_") + locale);
	a.installTranslator(&translator);

	MainWindow w;
	w.show();

	return a.exec();
}
