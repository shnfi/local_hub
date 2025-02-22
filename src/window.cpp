#include "window.h"
#include "../resources/stylesheets.cpp"
#include "win_inf.cpp"

#include "server.cpp"

Window::Window(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(CHAT_WIN_INF.WIN_TITLE);
    setFixedSize(CHAT_WIN_INF.WIN_W, CHAT_WIN_INF.WIN_H);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    ask_for_ip();

    /*
        * handling the fully exit if exit 
        * button of the IP window was pressed.
        */

    if (exit_on_dialog)
    {
        QTimer::singleShot(3, this, [=]() {
            close();
        });
    }

    // main layout

    main_layout = new QVBoxLayout(this);
    main_layout->setAlignment(Qt::AlignCenter);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);

    // title bar

    title_bar = new QWidget(this);
    title_bar->setFixedHeight(30);
    title_bar->setStyleSheet(title_bar_ss);
    main_layout->addWidget(title_bar);

    title_bar_layout = new QHBoxLayout(title_bar);
    title_bar_layout->setContentsMargins(0, 0, 0, 0);
    title_bar_layout->setAlignment(Qt::AlignCenter);

    buttons_widget = new QWidget(this);
    buttons_widget->setFixedHeight(30);

    buttons_layout = new QHBoxLayout(buttons_widget);
    buttons_layout->setAlignment(Qt::AlignCenter);

    exit_button = new QPushButton();
    maxmize_button = new QPushButton();
    minimize_button = new QPushButton();

    title_label = new QLabel(CHAT_WIN_INF.WIN_TITLE);
    title_label->setStyleSheet(title_label_ss);
    title_label->setAlignment(Qt::AlignCenter);

    connect(exit_button, &QPushButton::clicked, this, [=]() { close(); });
    connect(minimize_button, &QPushButton::clicked, this, [=]() { showMinimized(); });

    exit_button->setStyleSheet(exit_button_ss);
    exit_button->setFixedSize(14, 14);
    buttons_layout->addWidget(exit_button);
    
    maxmize_button->setStyleSheet(maxmize_button_ss);
    maxmize_button->setFixedSize(14, 14);
    buttons_layout->addWidget(maxmize_button);

    minimize_button->setStyleSheet(minimize_button_ss);
    minimize_button->setFixedSize(14, 14);
    buttons_layout->addWidget(minimize_button);

    title_bar_layout->addWidget(buttons_widget);
    title_bar_layout->addWidget(title_label);
    title_bar_layout->addSpacerItem(new QSpacerItem(50, 30));

    title_bar_layout->setStretch(0, 1);
    title_bar_layout->setStretch(1, 5);
    title_bar_layout->setStretch(2, 1);

    // ip displayer container

    ip_displayer_widget = new QWidget(this);
    ip_displayer_widget->setStyleSheet(ip_displayer_widget_ss);
    ip_displayer_widget->setObjectName("ip_displayer_widget");
    main_layout->addWidget(ip_displayer_widget);

    ip_displayer_layout = new QVBoxLayout(ip_displayer_widget);
    ip_displayer_layout->setAlignment(Qt::AlignCenter);

    ip_label = new QLabel(contact_ip, this);
    ip_label->setStyleSheet(ip_label_ss);
    ip_displayer_layout->addWidget(ip_label);

    // chat container 

    chat_container_widget = new QWidget(this);
    chat_container_widget->setStyleSheet(chat_container_widget_ss);

    chat_container_layout = new QVBoxLayout(chat_container_widget);

    chat_container_scroll_area = new QScrollArea();
    chat_container_scroll_area->setWidget(chat_container_widget);
    chat_container_scroll_area->setWidgetResizable(true);
    chat_container_scroll_area->setContentsMargins(0, 0, 0, 0);
    chat_container_scroll_area->setFrameShape(QFrame::NoFrame);
    main_layout->addWidget(chat_container_scroll_area);

    // message field container

    message_field_container_widget = new QWidget(this);
    message_field_container_widget->setStyleSheet(message_field_container_widget_ss);
    main_layout->addWidget(message_field_container_widget);

    message_field_container_layout = new QHBoxLayout(message_field_container_widget);

    message_field = new QLineEdit();
    message_field->setPlaceholderText("Enter your message");
    message_field->setFixedHeight(40);
    message_field->setStyleSheet(message_field_ss);

    send_button = new QPushButton("Send");
    send_button->setFixedHeight(40);
    send_button->setStyleSheet(send_button_ss);

    message_field_container_layout->addWidget(message_field);
    message_field_container_layout->addWidget(send_button);

    message_field_container_layout->setStretch(0, 6);
    message_field_container_layout->setStretch(1, 1);

    main_layout->setStretch(0, 1);
    main_layout->setStretch(1, 2);
    main_layout->setStretch(2, 20);
    main_layout->setStretch(3, 2);
}

Window::~Window() {}

void Window::ask_for_ip()
{
    dialog = new QDialog(this);
    dialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    dialog->setStyleSheet(dialog_ss);
    dialog->setWindowTitle(IP_WIN_INF.WIN_TITLE);
    dialog->setFixedSize(IP_WIN_INF.WIN_W, IP_WIN_INF.WIN_H);

    dialog_layout = new QVBoxLayout(dialog);
    dialog_layout->setAlignment(Qt::AlignCenter);
    dialog_layout->setContentsMargins(0, 0, 0, 0);

    title_bar = new QWidget();
    title_bar->setFixedHeight(30);
    title_bar->setStyleSheet(title_bar_ss);
    dialog_layout->addWidget(title_bar);

    form_widget = new QWidget();
    dialog_layout->addWidget(form_widget);

    dialog_layout->setStretch(0, 1);
    dialog_layout->setStretch(1, 11);

    form_layout = new QVBoxLayout(form_widget);
    form_layout->setAlignment(Qt::AlignCenter);

    title_bar_layout = new QHBoxLayout(title_bar);
    title_bar_layout->setContentsMargins(0, 0, 0, 0);
    title_bar_layout->setAlignment(Qt::AlignCenter);

    buttons_widget = new QWidget();
    buttons_widget->setFixedHeight(30);

    buttons_layout = new QHBoxLayout(buttons_widget);
    buttons_layout->setAlignment(Qt::AlignCenter);

    exit_button = new QPushButton();
    maxmize_button = new QPushButton();
    minimize_button = new QPushButton();

    title_label = new QLabel(IP_WIN_INF.WIN_TITLE);
    title_label->setStyleSheet(title_label_ss);
    title_label->setAlignment(Qt::AlignCenter);

    exit_button->setStyleSheet(exit_button_ss);
    exit_button->setFixedSize(14, 14);
    buttons_layout->addWidget(exit_button);
    
    maxmize_button->setStyleSheet(maxmize_button_ss);
    maxmize_button->setFixedSize(14, 14);
    buttons_layout->addWidget(maxmize_button);

    minimize_button->setStyleSheet(minimize_button_ss);
    minimize_button->setFixedSize(14, 14);
    buttons_layout->addWidget(minimize_button);

    connect(exit_button, &QPushButton::clicked, this, [=]() {
        exit_on_dialog = true;
        dialog->close();
    });

    connect(minimize_button, &QPushButton::clicked, this, [=]() { dialog->showMinimized(); });

    title_bar_layout->addWidget(buttons_widget);
    title_bar_layout->addWidget(title_label);
    title_bar_layout->addSpacerItem(new QSpacerItem(50, 30));

    title_bar_layout->setStretch(0, 1);
    title_bar_layout->setStretch(1, 5);
    title_bar_layout->setStretch(2, 1);

    ip_field = new QLineEdit(dialog);
    ip_field->setStyleSheet(ip_field_ss);
    ip_field->setFixedWidth(350);
    ip_field->setPlaceholderText("Enter the IP address");
    form_layout->addWidget(ip_field);

    form_layout->addSpacerItem(new QSpacerItem(20, 20));

    submit_button = new QPushButton("Submit", dialog);
    submit_button->setStyleSheet(submit_button_ss);
    submit_button->setFixedWidth(350);
    form_layout->addWidget(submit_button);

    connect(submit_button, &QPushButton::clicked, this, [=]() {
        contact_ip = ip_field->text();
        start_server(contact_ip.toStdString());
        dialog->close();
    });

    dialog->setLayout(dialog_layout);
    dialog->exec();
}