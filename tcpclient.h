#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#pragma once
#include <iostream>
#include <boost/asio.hpp>

class MainWindow;

class TcpClient
{
public:
    TcpClient(MainWindow& mainWin);
    ~TcpClient();
    std::string connectSocket(std::string, int);
    std::string sendInitMessage(std::string username);
    void runContext();
    void readNextMessage();
private:
    boost::asio::io_context            m_io_context;
    boost::asio::ip::tcp::socket       m_socket;
    boost::asio::streambuf             m_streambuf;
    std::thread                        m_runThread;
    MainWindow&                        m_mainWindow;
};

#endif // TCPCLIENT_H
