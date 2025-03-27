#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onBanUser();
    void onKickUser();
    void refreshMessages();
    void refreshUsers();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *messagesModel;
    QSqlTableModel *usersModel;
};

#endif // MAINWINDOW_H
