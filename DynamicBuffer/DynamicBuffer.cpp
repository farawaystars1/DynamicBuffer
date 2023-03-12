//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include<boost/asio.hpp>
#include<boost/bind/bind.hpp>
#include<memory>
#include<string>
#include<iostream>
#include"Buffer/Buffer.h"
#include<functional>
#include<fstream>
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

void fail(system::error_code ec, const char* reason)
{
    std::cout << reason << ':' << ec.message() << std::endl;
    return;
}
class tcp_connection
    : public std::enable_shared_from_this<tcp_connection>
{
public:
    typedef std::shared_ptr<tcp_connection> pointer;
    tcp_connection(tcp::socket &&socket)
        : socket_(std::move(socket)), inBuf_(new Buffer()), outBuf_(new Buffer()),outBuf_2_(new SimpleBuffer()),outfile("/mnt/c/users/administrator/desktop/test5.flac",std::ios::binary|std::ios::out)
    {
        isDealingMessage_ = false;
        isSendingMessage_ = false;
        //    onMessageCallback_ = std::bind(&tcp_connection::onMessage, shared_from_this(), std::placeholders::_1, std::placeholders::_2);

    }
    ~tcp_connection()
    {
        delete inBuf_;
        inBuf_ = nullptr;
        delete outBuf_;
        outBuf_ = nullptr;
    }
    void start()
    {
        
        if (inBuf_->writableBytes() < 1024) {
         
            inBuf_->recapacity(inBuf_->capacity() + 1024);
   
        }
        socket_.async_read_some(boost::asio::buffer(inBuf_->data_ + inBuf_->writePos_, 1024),
            boost::bind(&tcp_connection::handle_read, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        
    }
    void onMessage(pointer ptr, Buffer*buf) {
        std::string str=buf->readAllAsString();
        outfile.write(str.data(), str.size());
        ptr->sendMessage(str.data(), str.size());

    }

    //处理发送信息
    void sendMessage(char* data, size_t n)
    {
        outBuf_->append(data, n);
        dealSend();
    }
private:
    void dealSend()
    {

        if (isSendingMessage_==false)
        {
            isSendingMessage_ = true;
            //补充outBuf_2数据
            size_t n = outBuf_->readableBytes();
            if (n == 0) {
                isSendingMessage_ = false;
                return;
            }
            if (outBuf_2_->capacity() <n)
            {
                outBuf_2_->resize(n);
            }
            outBuf_->read(outBuf_2_->data_, n);
            outBuf_2_->readPos_ = 0;
            outBuf_2_->writePos_= n;
            //send
            std::cout << "start send..\n";
        
            std::cout <<"buf2 readable bytes:"<< outBuf_2_->readableBytes() << std::endl;


                socket_.async_write_some(buffer(outBuf_2_->data_ + outBuf_2_->readPos_, outBuf_2_->readableBytes()), boost::bind(&tcp_connection::onSend, shared_from_this(), asio::placeholders::error, asio::placeholders::bytes_transferred));
            
        
    
        }


    }
    void onSend(system::error_code ec, size_t transferred_bytes)
    {
        outBuf_2_->readPos_ += transferred_bytes;
        std::cout << "end send...\n";
        if (outBuf_2_->readableBytes() == 0)
        {
            isSendingMessage_ = false;
            dealSend();
            return;
        }
            socket_.async_write_some(buffer(outBuf_2_->data_ + outBuf_2_->readPos_, outBuf_2_->readableBytes()), boost::bind(&tcp_connection::onSend, shared_from_this(), asio::placeholders::error, asio::placeholders::bytes_transferred));


    }

  

    void handle_read(const boost::system::error_code& error,
        size_t bytes_transferred)
    {
        if (error)
        {
            fail(error, "read");
            onClosed();
           
            return;
        }
       
      
        inBuf_->writePos_ += bytes_transferred;
        
        if (!isDealingMessage_)
        {
            isDealingMessage_ = true;
           // onMessageCallback_(shared_from_this(), inBuf_);
      
            onMessage(shared_from_this(), inBuf_);
            isDealingMessage_ = false;
        }
        start();

    }
    void onClosed()
    {
        std::cout << "close.....\n";
    }
private:
    std::ofstream  outfile;
    tcp::socket socket_;
    //输入一次缓存 输出 两次缓存
    Buffer* inBuf_;
    Buffer* outBuf_;
    SimpleBuffer *outBuf_2_;
    std::function<void(pointer, Buffer*)> onMessageCallback_;
    std::atomic<bool> isDealingMessage_;
    std::atomic<bool> isSendingMessage_;
  

};

class tcp_server
{
public:
    tcp_server(boost::asio::io_context& io_context,uint16_t port)
        : io_context_(io_context),new_socket_(io_context),
        acceptor_(io_context, tcp::endpoint(tcp::v4(),port))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        
        acceptor_.async_accept(new_socket_,
            boost::bind(&tcp_server::handle_accept,this,
                boost::asio::placeholders::error));
    }

    void handle_accept(
        const boost::system::error_code& error)
    {
        if (error)
        {
            fail(error, "handle_accept");
            return;
        }
        tcp_connection::pointer(new tcp_connection(std::move(new_socket_)))->start();
        start_accept();
    }
private:
    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
    tcp::socket new_socket_;
};

int main(int argc,char**argv)
{
    try
    {
        if (argc < 2)
        {
            std::cout << "Usage <port>" << std::endl;
            return 0;
        }
        io_context io_context;
        tcp_server server(io_context,atoi(argv[1]));
        std::this_thread::sleep_for(std::chrono::seconds(15));
        io_context.run();
    }
    catch (std::runtime_error er)
    {
        std::cout << er.what() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}