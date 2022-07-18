#pragma once

#include <boost/asio/dispatch.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <memory>

#include "frontcontroller.h"
#include "request.h"

namespace dtl {
class Session : public std::enable_shared_from_this<Session> {
   private:
    boost::beast::tcp_stream stream_;
    boost::beast::flat_buffer buffer_;
    boost::beast::http::request<boost::beast::http::string_body> request_;
    boost::beast::http::response<boost::beast::http::string_body> response_;
    FrontController controller_;
    void Read() {
        request_ = {};
        stream_.expires_after(std::chrono::seconds(30));
        auto handler = &Session::Write;
        namespace beast = boost::beast;
        auto bind = beast::bind_front_handler(handler, shared_from_this());
        beast::http::async_read(stream_, buffer_, request_, std::move(bind));
    }
    void Write(boost::beast::error_code error, size_t bytes_transferred) {
        if (error == boost::beast::http::error::end_of_stream) return Close();
        if (error) {
            return;
        }
        namespace beast = boost::beast;
        namespace http = beast::http;
        using rq = http::request<http::string_body>;
        using rs = http::response<http::string_body>;
        response_ = controller_.Handle<rq, rs>(request_);
        auto handler = &Session::Resolve;
        auto this_sp = shared_from_this();
        auto bind =
            beast::bind_front_handler(handler, this_sp, response_.need_eof());
        http::async_write(stream_, response_, std::move(bind));
    }
    void Resolve(bool close, boost::beast::error_code error,
                 std::size_t bytes_transferred) {
        if (error) {
            return;
        }
        if (close) return Close();
        response_ = {};
        Read();
    }
    void Close() {
        boost::beast::error_code error;
        stream_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send,
                                  error);
    }

   public:
    Session(boost::asio::ip::tcp::socket &&socket, FrontController controller)
        : stream_(std::move(socket)), controller_(controller) {}
    void Run() {
        auto executor = stream_.get_executor();
        auto handler = &Session::Read;
        namespace beast = boost::beast;
        auto bind = beast::bind_front_handler(handler, shared_from_this());
        boost::asio::dispatch(executor, std::move(bind));
    }
};
}  // namespace dtl
