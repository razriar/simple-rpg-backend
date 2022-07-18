#pragma once

#include <boost/beast/http.hpp>
#include <string>

namespace dtl {
class Response {
   private:
    std::string body_;

   public:
    Response() { body_ = "Target not found"; }
    std::string body() { return body_; }
    std::string body(const std::string &other_body) {
        body_ = other_body;
        return body_;
    }
    operator boost::beast::http::response<boost::beast::http::string_body>() {
        namespace http = boost::beast::http;
        auto response = http::response<http::string_body>();
        response.body() = body_;
        return response;
    }
};
}  // namespace dtl
