#pragma once

#include <boost/beast/http.hpp>
#include <map>
#include <string>

#include "method.h"
#include "target.h"

namespace dtl {
class Request {
   private:
    std::string body_;
    Target target_;
    Method method_;
    std::map<std::string, Method> methods_;

   public:
    Request(const boost::beast::http::request<boost::beast::http::string_body>
                &request) {
        methods_["GET"] = Method::Get;
        methods_["POST"] = Method::Post;
        auto method_string = std::string(request.method_string());
        if (methods_.contains(method_string))
            method_ = methods_[method_string];
        else
            method_ = Method::Unknown;
        target_ = std::string(request.target());
        body_ = request.body();
    }
    std::string body() const { return body_; }
    Target target() const { return target_; }
    Method method() const { return method_; }
};
}  // namespace dtl
