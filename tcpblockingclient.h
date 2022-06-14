// https://www.boost.org/doc/libs/1_52_0/doc/html/boost_asio/example/timeouts/blocking_tcp_client.cpp

#pragma once

#include <boost/asio/connect.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/system/system_error.hpp>
#include <boost/asio/write.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "log.h"

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;
using boost::lambda::bind;
using boost::lambda::var;
using boost::lambda::_1;

class TcpBlockingClient
{
public:
  TcpBlockingClient()
    : socket_(io_service_),
      deadline_(io_service_)
  {
    deadline_.expires_at(boost::posix_time::pos_infin);
    check_deadline();
  }

  ~TcpBlockingClient()
  {
      LOG("TcpBlockingClient deleted")
  }

  void connect(const std::string& host, const std::string& service,
      boost::posix_time::time_duration timeout)
  {
    tcp::resolver::query query(host, service);
    tcp::resolver::iterator iter = tcp::resolver(io_service_).resolve(query);
    deadline_.expires_from_now(timeout);
    boost::system::error_code ec = boost::asio::error::would_block;
    boost::asio::async_connect(socket_, iter, var(ec) = _1);
    do io_service_.run_one(); while (ec == boost::asio::error::would_block);
    if (ec || !socket_.is_open())
    {
//      throw boost::system::system_error(
//          ec ? ec : boost::asio::error::operation_aborted);
        LOG("connect error: "<<ec.message())
    }
  }

  std::string readLine(boost::posix_time::time_duration timeout, char delimeter = '\n')
  {
    deadline_.expires_from_now(timeout);
    boost::system::error_code ec = boost::asio::error::would_block;
    boost::asio::async_read_until(socket_, input_buffer_, delimeter, var(ec) = _1);
    do io_service_.run_one(); while (ec == boost::asio::error::would_block);

    if (ec)
    {
        LOG("readline error: " << ec.message())

        throw boost::system::system_error(ec);
    }

    std::string line;
    std::istream is(&input_buffer_);
    std::getline(is, line, delimeter);
    return line;
  }

  void sendCommandMessage(const std::string& command, const std::string& arg2,
      boost::posix_time::time_duration timeout, char delimeter = '\n') // may be used as one with sendMsgToEverybody
  {
    std::string message = command+ '\n' + arg2 + delimeter;
    deadline_.expires_from_now(timeout);
    boost::system::error_code ec = boost::asio::error::would_block;
    boost::asio::async_write(socket_, boost::asio::buffer(message), var(ec) = _1);
    do io_service_.run_one(); while (ec == boost::asio::error::would_block);
    if (ec)
    {
      LOG("sendCommandMessage error: "<<ec.message())
    }
  }

  void close()
  {
      socket_.close();
  }

private:
  void check_deadline()
  {
    if (deadline_.expires_at() <= deadline_timer::traits_type::now())
    {
      boost::system::error_code ignored_ec;
      socket_.close(ignored_ec);
      deadline_.expires_at(boost::posix_time::pos_infin);
    }
    deadline_.async_wait(bind(&TcpBlockingClient::check_deadline, this));
  }

  boost::asio::io_service io_service_;
  tcp::socket socket_;
  deadline_timer deadline_;
  boost::asio::streambuf input_buffer_;
};
