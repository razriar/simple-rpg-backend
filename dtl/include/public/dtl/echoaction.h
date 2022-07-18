#pragma once

#include "request.h"
#include "response.h"
#include "target.h"

namespace dtl {
class EchoAction {
   public:
    static Target Target() { return std::string("/echo"); }
    static Response Get(const Request &request) {
        auto response = Response();
        response.body(request.body());
        return response;
    }
};
}  // namespace dtl
