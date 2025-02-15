#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QStackedWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QTimer>
#include <QScrollArea>

#include "chat_scr.cpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Chat_Window chat_win;
	chat_win.show();

	return app.exec();
}
