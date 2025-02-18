#ifndef WINDOW_H
#define WINDOW_H

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

class Window : public QWidget
{
private:
    QString contact_ip = "";
	bool exit_on_dialog = false;

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

private:
    void ask_for_ip();

private:
    QVBoxLayout *main_layout;
    QWidget *title_bar;
    QHBoxLayout *title_bar_layout;
    QWidget *buttons_widget;
    QHBoxLayout *buttons_layout;
    QPushButton *exit_button;
    QPushButton *maxmize_button;
    QPushButton *minimize_button;
    QLabel *title_label;
    QWidget *ip_displayer_widget;
    QVBoxLayout *ip_displayer_layout;
    QLabel *ip_label;
    QWidget *chat_container_widget;
    QVBoxLayout *chat_container_layout;
    QScrollArea *chat_container_scroll_area;
    QWidget *message_field_container_widget;
    QHBoxLayout *message_field_container_layout;
    QLineEdit *message_field;
    QPushButton *send_button;
    QDialog *dialog;
    QVBoxLayout *dialog_layout;
    QWidget *form_widget;
    QVBoxLayout *form_layout;
    QLineEdit *ip_field;
    QPushButton *submit_button;
};

#endif WINDOW_H