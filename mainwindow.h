#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpblockingclient.h"
#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void acceptUserList(std::string& userList);

private slots:
    void on_connectButton_clicked();

    void on_sendButton_clicked();

    void on_disconnectButton_clicked();

private:
    Ui::MainWindow      *ui;
    TcpBlockingClient   m_client;
    std::thread         m_readThread;
};

#endif // MAINWINDOW_H
