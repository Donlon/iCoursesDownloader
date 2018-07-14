#include "mainwnd.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWnd mainWnd;
	mainWnd.show();
	mainWnd.hide();

	return a.exec();
}
