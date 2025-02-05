#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "config.h"

#include <QVBoxLayout>
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QSpacerItem>
#include <QKeyEvent>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>

#include <QtGlobal>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: rgba(0, 0, 0, 200);");
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint);
    this->showFullScreen();

    QLineEdit *passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setPlaceholderText("Введите пароль");
    passwordInput->setFixedSize(200, 40);
    passwordInput->setStyleSheet("background-color: rgb(255, 255, 255);");

    QPushButton *checkButton = new QPushButton("Ввести", this);
    checkButton->setFixedSize(100, 40);
    checkButton->setStyleSheet("background-color: rgb(255, 255, 255);");

    QVBoxLayout *layout = new QVBoxLayout();

    QSpacerItem *topSpacer = new QSpacerItem(20, 200, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *bottomSpacer = new QSpacerItem(20, 200, QSizePolicy::Minimum, QSizePolicy::Expanding);

    layout->addItem(topSpacer);

    layout->addWidget(passwordInput, 0, Qt::AlignCenter);
    layout->addWidget(checkButton, 0, Qt::AlignCenter);

    layout->addItem(bottomSpacer);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(20);

    this->setCentralWidget(centralWidget);

    QString correctPassword = "1234";

    connect(checkButton, &QPushButton::clicked, this, [=]() {
        if (passwordInput->text() == correctPassword) {
            Config::password_correct=true;
            this->close();
        } else {
            passwordInput->setPlaceholderText("Неверный пароль");
            passwordInput->setText("");
        }
    });

}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Super_L || ev->key() == Qt::Key_Super_R) {
        ev->ignore();
        return;
    }

    QMainWindow::keyPressEvent(ev);
}

void MainWindow::closeEvent(QCloseEvent  *ev)
{
    if(!Config::password_correct){
        ev->ignore();
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
