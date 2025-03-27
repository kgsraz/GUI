#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Подключение к PostgreSQL
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("messenger");
    db.setUserName("postgres");
    db.setPassword("your_password");
    if (!db.open()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к БД");
        return;
    }

    // Настройка модели сообщений
    messagesModel = new QSqlTableModel(this, db);
    messagesModel->setTable("messages");
    messagesModel->setHeaderData(1, Qt::Horizontal, "Отправитель");
    messagesModel->setHeaderData(2, Qt::Horizontal, "Получатель");
    messagesModel->setHeaderData(3, Qt::Horizontal, "Сообщение");
    messagesModel->setHeaderData(4, Qt::Horizontal, "Время");
    messagesModel->select();
    ui->messagesView->setModel(messagesModel);
    ui->messagesView->hideColumn(0);
    ui->messagesView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Настройка модели пользователей
    usersModel = new QSqlTableModel(this, db);
    usersModel->setTable("users");
    usersModel->setHeaderData(1, Qt::Horizontal, "Имя");
    usersModel->setHeaderData(4, Qt::Horizontal, "Статус");
    usersModel->select();
    ui->usersView->setModel(usersModel);
    ui->usersView->hideColumn(0);
    ui->usersView->hideColumn(2);
    ui->usersView->hideColumn(3);
    ui->usersView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Привязка кнопок
    connect(ui->banButton, &QPushButton::clicked, this, &MainWindow::onBanUser);
    connect(ui->kickButton, &QPushButton::clicked, this, &MainWindow::onKickUser);
    connect(ui->refreshMessagesButton, &QPushButton::clicked, this, &MainWindow::refreshMessages);
    connect(ui->refreshUsersButton, &QPushButton::clicked, this, &MainWindow::refreshUsers);

    // Стилизация
    setStyleSheet(R"(
        QMainWindow { background: #2d2d2d; }
        QTableView { alternate-background-color: #3d3d3d; color: white; }
        QPushButton { background: #5a5a5a; color: white; padding: 8px; border-radius: 4px; }
        QPushButton:hover { background: #7a7a7a; }
        QTabBar::tab { background: #4a4a4a; color: white; padding: 10px; }
        QTabBar::tab:selected { background: #5a5a5a; }
    )");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onBanUser() {
    QModelIndex index = ui->usersView->currentIndex();
    if (!index.isValid()) return;

    int userId = usersModel->data(usersModel->index(index.row(), 0)).toInt();
    QString username = usersModel->data(usersModel->index(index.row(), 1)).toString();

    if (QMessageBox::question(this, "Бан", "Забанить " + username + "?") == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("UPDATE users SET banned = true WHERE id = ?");
        query.addBindValue(userId);
        if (!query.exec()) {
            QMessageBox::critical(this, "Ошибка", "Не удалось забанить пользователя");
        }
        usersModel->select();
    }
}

void MainWindow::onKickUser() {
    QMessageBox::information(this, "Отключение", "Функция в разработке");
}

void MainWindow::refreshMessages() {
    messagesModel->select();
}

void MainWindow::refreshUsers() {
    usersModel->select();
}
