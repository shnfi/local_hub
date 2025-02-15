#include "win_inf.cpp"
#include "stylesheets.cpp"

class Chat_Window : public QWidget
{
private:
	QString contact_ip = "";
	bool exit_on_dialog = false;

public: 
	Chat_Window(QWidget *parent = nullptr) : QWidget(parent)
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
	}

private:
	void ask_for_ip()
	{
		QDialog *dialog = new QDialog(this);
		dialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		dialog->setStyleSheet(dialog_ss);
		dialog->setWindowTitle(IP_WIN_INF.WIN_TITLE);
		dialog->setFixedSize(IP_WIN_INF.WIN_W, IP_WIN_INF.WIN_H);

		QVBoxLayout *dialog_layout = new QVBoxLayout(dialog);
		dialog_layout->setAlignment(Qt::AlignCenter);
		dialog_layout->setContentsMargins(0, 0, 0, 0);

		QWidget *title_bar = new QWidget();
		title_bar->setFixedHeight(30);
		title_bar->setStyleSheet(title_bar_ss);
		dialog_layout->addWidget(title_bar);

		QWidget *form_widget = new QWidget();
		dialog_layout->addWidget(form_widget);

		dialog_layout->setStretch(0, 1);
		dialog_layout->setStretch(1, 11);

		QVBoxLayout *form_layout = new QVBoxLayout(form_widget);
		form_layout->setAlignment(Qt::AlignCenter);

		QHBoxLayout *title_bar_layout = new QHBoxLayout(title_bar);
		title_bar_layout->setContentsMargins(0, 0, 0, 0);
		title_bar_layout->setAlignment(Qt::AlignCenter);

		QWidget *buttons_widget = new QWidget();
		buttons_widget->setFixedHeight(30);

		QHBoxLayout *buttons_layout = new QHBoxLayout(buttons_widget);
		buttons_layout->setAlignment(Qt::AlignCenter);

		QPushButton *exit_button = new QPushButton();
		QPushButton *maxmize_button = new QPushButton();
		QPushButton *minimize_button = new QPushButton();

		QLabel *title_label = new QLabel(IP_WIN_INF.WIN_TITLE);
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

		QLineEdit *ip_field = new QLineEdit(dialog);
		ip_field->setStyleSheet(ip_field_ss);
		ip_field->setFixedWidth(350);
		ip_field->setPlaceholderText("Enter the IP address");
		form_layout->addWidget(ip_field);

		form_layout->addSpacerItem(new QSpacerItem(20, 20));

		QPushButton *submit_button = new QPushButton("Submit", dialog);
		submit_button->setStyleSheet(submit_button_ss);
		submit_button->setFixedWidth(350);
		form_layout->addWidget(submit_button);

		connect(submit_button, &QPushButton::clicked, this, [=]() {
			contact_ip = ip_field->text();
			dialog->close();
		});

		dialog->setLayout(dialog_layout);
		dialog->exec();
	}
};
