#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <memory>

#include "iocontext.h"
#include "sessionfactory.h"

namespace dtl {
class Listener : public std::enable_shared_from_this<Listener> {
   private:
    IOContext io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    SessionFactory factory_;

    void Accept(boost::beast::error_code error,
                boost::asio::ip::tcp::socket socket) {
        if (error) {
            std::cerr << error.value() << std::endl;
            return;
        }
        factory_.RunSession(std::move(socket));
        Listen();
    }
    void Listen() {
        auto strand = boost::asio::make_strand(io_context_.data());
        auto handler = &Listener::Accept;
        namespace beast = boost::beast;
        auto bind = beast::bind_front_handler(handler, shared_from_this());
        acceptor_.async_accept(strand, std::move(bind));
    }

   public:
    Listener(IOContext& io_context, boost::asio::ip::tcp::acceptor& acceptor,
             SessionFactory factory)
        : io_context_(io_context),
          acceptor_(std::move(acceptor)),
          factory_(factory) {}
    Listener(Listener& other)
        : io_context_(other.io_context_),
          acceptor_(std::move(other.acceptor_)),
          factory_(other.factory_) {}
    void Run() { Listen(); }
};
}  // namespace dtl
