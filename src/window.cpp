#include "window.h"
#include "../resources/stylesheets.cpp"
#include "win_inf.cpp"

#include "server.cpp"

Window::Window(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(CHAT_WIN_INF.WIN_TITLE);
    setFixedSize(CHAT_WIN_INF.WIN_W, CHAT_WIN_INF.WIN_H);
    setStyleSheet("background-color: transparent;");

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

    // frame

    main_frame = new QFrame(this);
    main_frame->setFixedSize(CHAT_WIN_INF.WIN_W, CHAT_WIN_INF.WIN_H);
    main_frame->setStyleSheet(frame_ss);

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
    chat_container_layout->addStretch();

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

    connect(message_field, &QLineEdit::textEdited, this, &Window::check_for_field_val);

    send_button = new QPushButton("Send");
    send_button->setFixedHeight(40);
    send_button->setStyleSheet(send_button_ss);

    clear_the_field();

    connect(send_button, &QPushButton::clicked, this, [=]() {
        send_msg(message_field->text(), client_socket);
        clear_the_field();
    });

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

    dialog_frame = new QFrame(dialog);
    dialog_frame->setFixedSize(IP_WIN_INF.WIN_W, IP_WIN_INF.WIN_H);
    dialog_frame->setStyleSheet(frame_ss);

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

void Window::check_for_field_val()
{
    if (message_field->text() == "")
        send_button->hide();
    else
        send_button->show();
}

void Window::clear_the_field()
{
    message_field->clear();
    send_button->hide();
}

void Window::send_msg(QString msg, int cs)
{
    if (send(cs, msg.toStdString().c_str(), msg.size(), 0) == -1)
    {
        std::cout << "[x] message did not sent!" << std::endl;
        return;
    }

    QWidget *message_widget = new QWidget(this);

    QHBoxLayout *message_widget_layout = new QHBoxLayout(message_widget);
    message_widget_layout->setAlignment(Qt::AlignRight);
    message_widget_layout->setContentsMargins(0, 0, 0, 0);

    QWidget *message_container = new QWidget();
    message_container->setMinimumHeight(40);
    message_container->setMinimumWidth(70);
    message_container->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    message_container->setStyleSheet(sending_message_widget_ss);
    message_widget_layout->addWidget(message_container);

    QHBoxLayout *message_layout = new QHBoxLayout(message_container);
    message_layout->setAlignment(Qt::AlignCenter);

    QLabel *message_text = new QLabel((QString) msg);
    message_layout->addWidget(message_text);

    chat_container_layout->addWidget(message_widget);
}

void Window::receive_msg(int ss)
{
    char buff[BUFFER_SIZE];

    if (recv(ss, buff, BUFFER_SIZE, 0) == -1)
    {
        std::cout << "[x] message did not received!" << std::endl;
        return;
    }

    QWidget *message_widget = new QWidget(this);

    QHBoxLayout *message_widget_layout = new QHBoxLayout(message_widget);
    message_widget_layout->setAlignment(Qt::AlignLeft);
    message_widget_layout->setContentsMargins(0, 0, 0, 0);

    QWidget *message_container = new QWidget();
    message_container->setMinimumHeight(40);
    message_container->setMinimumWidth(70);
    message_container->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    message_container->setStyleSheet(received_message_widget_ss);
    message_widget_layout->addWidget(message_container);

    QHBoxLayout *message_layout = new QHBoxLayout(message_container);
    message_layout->setAlignment(Qt::AlignCenter);

    QLabel *message_text = new QLabel((QString) buff);
    message_layout->addWidget(message_text);

    chat_container_layout->addWidget(message_widget);
}