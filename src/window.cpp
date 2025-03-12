#include <thread>

#include "window.h"
#include "../resources/light_stylesheet.cpp"
#include "../resources/dark_stylesheet.cpp"
#include "win_inf.cpp"

#include "server.cpp"
#include "network_check.c"

Window::Window(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(CHAT_WIN_INF.WIN_TITLE);
    setFixedSize(CHAT_WIN_INF.WIN_W, CHAT_WIN_INF.WIN_H);
    setStyleSheet("background-color: transparent;");

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    if (network_check())
        ask_for_ip();
    else
        network_err();

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

    // server_socket_notifier widget

    server_socket_notifier = new QSocketNotifier(new_socket, QSocketNotifier::Read, this);

    connect(server_socket_notifier, &QSocketNotifier::activated, this, &Window::receive_msg);

    std::cout << "listening on port " << PORT << std::endl;

    // main layout

    main_layout = new QVBoxLayout(this);
    main_layout->setAlignment(Qt::AlignCenter);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);

    // frame

    main_frame = new QFrame(this);
    main_frame->setFixedSize(CHAT_WIN_INF.WIN_W, CHAT_WIN_INF.WIN_H);

    // title bar

    title_bar = new QWidget(this);
    title_bar->setFixedHeight(30);
    main_layout->addWidget(title_bar);

    title_bar_layout = new QHBoxLayout(title_bar);
    title_bar_layout->setContentsMargins(0, 0, 0, 0);
    title_bar_layout->setAlignment(Qt::AlignCenter);

    buttons_widget = new QWidget(this);
    buttons_widget->setFixedHeight(30);
    buttons_widget->setFixedWidth(85);

    buttons_layout = new QHBoxLayout(buttons_widget);
    buttons_layout->setAlignment(Qt::AlignCenter);

    exit_button = new QPushButton();
    maxmize_button = new QPushButton();
    minimize_button = new QPushButton();

    title_label = new QLabel(CHAT_WIN_INF.WIN_TITLE);
    title_label->setAlignment(Qt::AlignCenter);

    connect(exit_button, &QPushButton::clicked, this, [=]() { close(); });
    connect(minimize_button, &QPushButton::clicked, this, [=]() { showMinimized(); });

    exit_button->setFixedSize(14, 14);
    buttons_layout->addWidget(exit_button);

    maxmize_button->setFixedSize(14, 14);
    buttons_layout->addWidget(maxmize_button);

    minimize_button->setFixedSize(14, 14);
    buttons_layout->addWidget(minimize_button);

    change_theme_widget = new QWidget();
    change_theme_widget->setFixedHeight(30);
    change_theme_widget->setFixedWidth(50);

    change_theme_layout = new QHBoxLayout(change_theme_widget);
    change_theme_layout->setAlignment(Qt::AlignCenter);

    change_theme_button = new QPushButton();

    change_theme_button->setStyleSheet(change_theme_button_ss_light);
    change_theme_button->setFixedSize(14, 14);
    change_theme_layout->addWidget(change_theme_button);

    connect(change_theme_button, &QPushButton::clicked, this, [=]() {
        if (theme == 1)
        {
            theme = 2;
            main_dark_theme();
        }
        else
        {
            theme = 1;
            main_light_theme();
        }
    });

    title_bar_layout->addWidget(buttons_widget);
    title_bar_layout->addWidget(title_label);
    title_bar_layout->addWidget(change_theme_widget);

    title_bar_layout->setStretch(0, 1);
    title_bar_layout->setStretch(1, 5);
    title_bar_layout->setStretch(2, 1);

    // ip displayer container

    ip_displayer_widget = new QWidget(this);
    ip_displayer_widget->setObjectName("ip_displayer_widget");
    main_layout->addWidget(ip_displayer_widget);

    ip_displayer_layout = new QVBoxLayout(ip_displayer_widget);
    ip_displayer_layout->setAlignment(Qt::AlignCenter);

    ip_label = new QLabel(contact_ip, this);
    ip_displayer_layout->addWidget(ip_label);

    // chat container

    chat_container_widget = new QWidget(this);

    chat_container_layout = new QVBoxLayout(chat_container_widget);
    chat_container_layout->addStretch();

    chat_container_scroll_area = new QScrollArea();
    chat_container_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    chat_container_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    chat_container_scroll_area->setWidget(chat_container_widget);
    chat_container_scroll_area->setWidgetResizable(true);
    chat_container_scroll_area->setContentsMargins(0, 0, 0, 0);
    chat_container_scroll_area->setFrameShape(QFrame::NoFrame);
    main_layout->addWidget(chat_container_scroll_area);

    // message field container

    message_field_container_widget = new QWidget(this);
    main_layout->addWidget(message_field_container_widget);

    message_field_container_layout = new QHBoxLayout(message_field_container_widget);

    message_field = new QLineEdit();
    message_field->setPlaceholderText("Enter your message");
    message_field->setFixedHeight(40);

    connect(message_field, &QLineEdit::textEdited, this, &Window::check_for_field_val);

    send_button = new QPushButton("");
    send_button->setFixedHeight(40);
    send_button->setIcon(QIcon("resources/top_arrow.png"));

    clear_the_field();

    connect(send_button, &QPushButton::clicked, this, [=]() {
        send_msg(message_field->text());
        clear_the_field();
    });

    message_field_container_layout->addWidget(message_field);
    message_field_container_layout->addWidget(send_button);

    message_field_container_layout->setStretch(0, 13);
    message_field_container_layout->setStretch(1, 1);

    main_layout->setStretch(0, 1);
    main_layout->setStretch(1, 2);
    main_layout->setStretch(2, 20);
    main_layout->setStretch(3, 2);

    if (theme == 1)
        main_light_theme();
    else
        main_dark_theme();
}

Window::~Window() {}

void Window::ask_for_ip()
{
    dialog = new QDialog(this);
    dialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    dialog->setStyleSheet(dialog_ss_light);
    dialog->setWindowTitle(IP_WIN_INF.WIN_TITLE);
    dialog->setFixedSize(IP_WIN_INF.WIN_W, IP_WIN_INF.WIN_H);

    dialog_frame = new QFrame(dialog);
    dialog_frame->setFixedSize(IP_WIN_INF.WIN_W, IP_WIN_INF.WIN_H);
    dialog_frame->setStyleSheet(frame_ss_light);

    dialog_layout = new QVBoxLayout(dialog);
    dialog_layout->setAlignment(Qt::AlignCenter);
    dialog_layout->setContentsMargins(0, 0, 0, 0);

    title_bar = new QWidget();
    title_bar->setFixedHeight(30);
    title_bar->setStyleSheet(title_bar_ss_light);
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
    buttons_widget->setFixedWidth(85);

    buttons_layout = new QHBoxLayout(buttons_widget);
    buttons_layout->setAlignment(Qt::AlignCenter);

    exit_button = new QPushButton();
    maxmize_button = new QPushButton();
    minimize_button = new QPushButton();

    title_label = new QLabel(IP_WIN_INF.WIN_TITLE);
    title_label->setStyleSheet(title_label_ss_light);
    title_label->setAlignment(Qt::AlignCenter);

    exit_button->setStyleSheet(exit_button_ss_light);
    exit_button->setFixedSize(14, 14);
    buttons_layout->addWidget(exit_button);

    maxmize_button->setStyleSheet(maxmize_button_ss_light);
    maxmize_button->setFixedSize(14, 14);
    buttons_layout->addWidget(maxmize_button);

    minimize_button->setStyleSheet(minimize_button_ss_light);
    minimize_button->setFixedSize(14, 14);
    buttons_layout->addWidget(minimize_button);

    connect(exit_button, &QPushButton::clicked, this, [=]() {
        exit_on_dialog = true;
        dialog->close();
    });

    connect(minimize_button, &QPushButton::clicked, this, [=]() { dialog->showMinimized(); });

    change_theme_widget = new QWidget();
    change_theme_widget->setFixedHeight(30);
    change_theme_widget->setFixedWidth(50);

    change_theme_layout = new QHBoxLayout(change_theme_widget);
    change_theme_layout->setAlignment(Qt::AlignCenter);

    change_theme_button = new QPushButton();

    change_theme_button->setStyleSheet(change_theme_button_ss_light);
    change_theme_button->setFixedSize(14, 14);
    change_theme_layout->addWidget(change_theme_button);

    connect(change_theme_button, &QPushButton::clicked, this, [=]() {
        if (theme == 1)
        {
            theme = 2;
            dialog_dark_theme();
        }
        else
        {
            theme = 1;
            dialog_light_theme();
        }
    });

    title_bar_layout->addWidget(buttons_widget);
    title_bar_layout->addWidget(title_label);
    title_bar_layout->addWidget(change_theme_widget);

    title_bar_layout->setStretch(0, 1);
    title_bar_layout->setStretch(1, 5);
    title_bar_layout->setStretch(2, 1);

    ip_field = new QLineEdit(dialog);
    ip_field->setStyleSheet(ip_field_ss_light);
    ip_field->setFixedWidth(350);
    ip_field->setFixedHeight(40);
    ip_field->setPlaceholderText("Enter the IP address");
    form_layout->addWidget(ip_field);

    form_layout->addSpacerItem(new QSpacerItem(20, 20));

    submit_button = new QPushButton("Submit", dialog);
    submit_button->setStyleSheet(submit_button_ss_light);
    submit_button->setFixedWidth(350);
    submit_button->setFixedHeight(40);
    form_layout->addWidget(submit_button);

    connect(submit_button, &QPushButton::clicked, this, [=]() {
        contact_ip = ip_field->text();

        std::thread server_thread(start_server);
        std::thread client_thread(start_client, contact_ip.toStdString());

        server_thread.join();
        client_thread.join();
        
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

void Window::send_msg(QString msg)
{
    if (send(client_socket, msg.toStdString().c_str(), msg.size(), 0) == -1)
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
    message_container->setStyleSheet(sending_message_widget_ss_light);
    message_widget_layout->addWidget(message_container);

    QHBoxLayout *message_layout = new QHBoxLayout(message_container);
    message_layout->setAlignment(Qt::AlignCenter);

    QLabel *message_text = new QLabel((QString) msg);
    message_layout->addWidget(message_text);

    chat_container_layout->addWidget(message_widget);
}

void Window::receive_msg()
{
    char buff[BUFFER_SIZE] = {0};
    ssize_t received_bytes = recv(new_socket, buff, BUFFER_SIZE - 1, 0);

    if (received_bytes <= 0)
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
    message_container->setStyleSheet(received_message_widget_ss_light);
    message_widget_layout->addWidget(message_container);

    QHBoxLayout *message_layout = new QHBoxLayout(message_container);
    message_layout->setAlignment(Qt::AlignCenter);

    QLabel *message_text = new QLabel((QString) buff);
    message_layout->addWidget(message_text);

    chat_container_layout->addWidget(message_widget);
}

void Window::dialog_light_theme()
{
    dialog->setStyleSheet(dialog_ss_light);
    dialog_frame->setStyleSheet(frame_ss_light);
    title_bar->setStyleSheet(title_bar_ss_light);
    title_label->setStyleSheet(title_label_ss_light);
    exit_button->setStyleSheet(exit_button_ss_light);
    maxmize_button->setStyleSheet(maxmize_button_ss_light);
    minimize_button->setStyleSheet(minimize_button_ss_light);
    change_theme_button->setStyleSheet(change_theme_button_ss_light);
    ip_field->setStyleSheet(ip_field_ss_light);
    submit_button->setStyleSheet(submit_button_ss_light);
}

void Window::dialog_dark_theme()
{
    dialog->setStyleSheet(dialog_ss_dark);
    dialog_frame->setStyleSheet(frame_ss_dark);
    title_bar->setStyleSheet(title_bar_ss_dark);
    title_label->setStyleSheet(title_label_ss_dark);
    exit_button->setStyleSheet(exit_button_ss_dark);
    maxmize_button->setStyleSheet(maxmize_button_ss_dark);
    minimize_button->setStyleSheet(minimize_button_ss_dark);
    change_theme_button->setStyleSheet(change_theme_button_ss_dark);
    ip_field->setStyleSheet(ip_field_ss_dark);
    submit_button->setStyleSheet(submit_button_ss_dark);
}

void Window::main_light_theme()
{
    main_frame->setStyleSheet(frame_ss_light);
    title_bar->setStyleSheet(title_bar_ss_light);
    title_label->setStyleSheet(title_label_ss_light);
    exit_button->setStyleSheet(exit_button_ss_light);
    maxmize_button->setStyleSheet(maxmize_button_ss_light);
    minimize_button->setStyleSheet(minimize_button_ss_light);
    change_theme_widget->setStyleSheet(change_theme_button_ss_light);
    ip_displayer_widget->setStyleSheet(ip_displayer_widget_ss_light);
    ip_label->setStyleSheet(ip_label_ss_light);
    chat_container_widget->setStyleSheet(chat_container_widget_ss_light);
    message_field_container_widget->setStyleSheet(message_field_container_widget_ss_light);
    message_field->setStyleSheet(message_field_ss_light);
    send_button->setStyleSheet(send_button_ss_light);
}

void Window::main_dark_theme()
{
    main_frame->setStyleSheet(frame_ss_dark);
    title_bar->setStyleSheet(title_bar_ss_dark);
    title_label->setStyleSheet(title_label_ss_dark);
    exit_button->setStyleSheet(exit_button_ss_dark);
    maxmize_button->setStyleSheet(maxmize_button_ss_dark);
    minimize_button->setStyleSheet(minimize_button_ss_dark);
    change_theme_widget->setStyleSheet(change_theme_button_ss_dark);
    ip_displayer_widget->setStyleSheet(ip_displayer_widget_ss_dark);
    ip_label->setStyleSheet(ip_label_ss_dark);
    chat_container_widget->setStyleSheet(chat_container_widget_ss_dark);
    message_field_container_widget->setStyleSheet(message_field_container_widget_ss_dark);
    message_field->setStyleSheet(message_field_ss_dark);
    send_button->setStyleSheet(send_button_ss_dark);
}

void Window::network_err()
{
    dialog = new QDialog(this);
    dialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    dialog->setStyleSheet(dialog_ss_light);
    dialog->setWindowTitle(NETERR_WIN_INF.WIN_TITLE);
    dialog->setFixedSize(NETERR_WIN_INF.WIN_W, NETERR_WIN_INF.WIN_H);

    dialog_frame = new QFrame(dialog);
    dialog_frame->setFixedSize(NETERR_WIN_INF.WIN_W, NETERR_WIN_INF.WIN_H);
    dialog_frame->setStyleSheet(frame_ss_light);

    dialog_layout = new QVBoxLayout(dialog);
    dialog_layout->setAlignment(Qt::AlignCenter);
    dialog_layout->setContentsMargins(0, 0, 0, 0);

    title_bar = new QWidget();
    title_bar->setFixedHeight(30);
    title_bar->setStyleSheet(title_bar_ss_light);
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
    buttons_widget->setFixedWidth(85);

    buttons_layout = new QHBoxLayout(buttons_widget);
    buttons_layout->setAlignment(Qt::AlignCenter);

    exit_button = new QPushButton();
    maxmize_button = new QPushButton();
    minimize_button = new QPushButton();

    title_label = new QLabel(NETERR_WIN_INF.WIN_TITLE);
    title_label->setStyleSheet(title_label_ss_light);
    title_label->setAlignment(Qt::AlignCenter);

    exit_button->setStyleSheet(exit_button_ss_light);
    exit_button->setFixedSize(14, 14);
    buttons_layout->addWidget(exit_button);

    maxmize_button->setStyleSheet(maxmize_button_ss_light);
    maxmize_button->setFixedSize(14, 14);
    buttons_layout->addWidget(maxmize_button);

    minimize_button->setStyleSheet(minimize_button_ss_light);
    minimize_button->setFixedSize(14, 14);
    buttons_layout->addWidget(minimize_button);

    connect(exit_button, &QPushButton::clicked, this, [=]() {
        exit_on_dialog = true;
        dialog->close();
    });

    connect(minimize_button, &QPushButton::clicked, this, [=]() { dialog->showMinimized(); });

    change_theme_widget = new QWidget();
    change_theme_widget->setFixedHeight(30);
    change_theme_widget->setFixedWidth(50);

    change_theme_layout = new QHBoxLayout(change_theme_widget);
    change_theme_layout->setAlignment(Qt::AlignCenter);

    change_theme_button = new QPushButton();

    change_theme_button->setStyleSheet(change_theme_button_ss_light);
    change_theme_button->setFixedSize(14, 14);
    change_theme_layout->addWidget(change_theme_button);

    connect(change_theme_button, &QPushButton::clicked, this, [=]() {
        if (theme == 1)
        {
            theme = 2;
            dialog_dark_theme();
        }
        else
        {
            theme = 1;
            dialog_light_theme();
        }
    });

    title_bar_layout->addWidget(buttons_widget);
    title_bar_layout->addWidget(title_label);
    title_bar_layout->addWidget(change_theme_widget);

    title_bar_layout->setStretch(0, 1);
    title_bar_layout->setStretch(1, 5);
    title_bar_layout->setStretch(2, 1);

    QLabel *label = new QLabel();
    QPixmap pixmap("resources/network_err.png");
    
    label->setPixmap(pixmap);
    label->setScaledContents(true);
    label->setFixedSize(150, 150);

    if (pixmap.isNull())
        std::cout << "pixmap not loaded!" << std::endl;

    form_layout->addWidget(label);

    dialog->setLayout(dialog_layout);
    dialog->exec();
}
