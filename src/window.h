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
#include <QSocketNotifier>

#define BUFFER_SIZE 1024

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
    void check_for_field_val();
    void clear_the_field();
    void send_msg(QString msg);
    void receive_msg();
    void dialog_light_theme();
    void dialog_dark_theme();
    void main_light_theme();
    void main_dark_theme();

private:
    int theme = 1;
    QSocketNotifier *server_socket_notifier;
    QVBoxLayout *main_layout;
    QFrame *main_frame;
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
    QFrame *dialog_frame;
    QVBoxLayout *dialog_layout;
    QWidget *form_widget;
    QVBoxLayout *form_layout;
    QWidget *change_theme_widget;
    QHBoxLayout *change_theme_layout;
    QPushButton *change_theme_button;
    QLineEdit *ip_field;
    QPushButton *submit_button;
    QSocketNotifier *client_socket_notifier;
};

#endif