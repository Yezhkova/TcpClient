#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log.h"
#include "tcpblockingclient.h"
#include <regex>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_client()
{
    ui->setupUi(this);
    connect(this, &MainWindow::signal, ui->feed, &QTextEdit::append);
    ui->sendButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    std::string my_username = ui->myUsernameEdit->text().toStdString();
    std::string address = ui->addressEdit->text().toStdString();
    std::string port = ui->portEdit->text().toStdString();
    m_client.connect(address, port, boost::posix_time::pos_infin);

    m_client.sendCommandMessage("init", my_username, boost::posix_time::pos_infin);
    LOG(my_username)
    //ui->connectButton->setEnabled(false);
    //in a separate thread
    ui->sendButton->setEnabled(true);

    m_readThread = std::thread( [this] {
        try
        {
            for(;;)
            {
                std::string command = m_client.readLine(boost::posix_time::pos_infin);

                if (command == "list")
                {
                    std::string userList = m_client.readLine(boost::posix_time::pos_infin);
                    acceptUserList(userList);

                }
                else if (command == "msg")
                {
                    // read receiver name;
                    std::string sender = m_client.readLine(boost::posix_time::pos_infin);
                    std::string textOfMessage = m_client.readLine(boost::posix_time::pos_infin, EOF);
                    textOfMessage = std::regex_replace(textOfMessage, std::regex("<"), "&lt;");
                    std::string tmp = "<html><span style=\"color:#0033cc;font-weight:bold\">"
                                      + sender + "</span>:<br><span style=\"white-space:pre\">"
                                      + textOfMessage + "</span></html>";
                    emit signal(QString::fromStdString(tmp));
                }

                else if (command == "retry")
                {
                    this->close();
                }
            }
        }
        catch ( std::runtime_error &e) // catch everything
        {
            LOG("Cannot read line: " << e.what())
            std::terminate();
        }

    } );
}

void MainWindow::acceptUserList(std::string& userList)
{
    ui->sessionsList->clear();
    char* str = &userList[0];
    char *token = strtok(str, ";");
    while (token != NULL)
    {
        ui->sessionsList->addItem(token);
        token = strtok(NULL, ";");
    }
}

void MainWindow::on_sendButton_clicked()
{
    std::string my_message = ui->typeMessage->toPlainText().toStdString();
    if(!my_message.empty())
    {
        m_client.sendCommandMessage("msg", my_message, boost::posix_time::pos_infin, EOF);
    }
    ui->typeMessage->clear();
}


void MainWindow::on_disconnectButton_clicked()
{
    this->close();
}


