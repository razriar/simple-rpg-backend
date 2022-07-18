#pragma once

#include <memory>

#include "frontcontroller.h"
#include "session.h"

namespace dtl {
class SessionFactory {
   private:
    FrontController controller_;

   public:
    SessionFactory(FrontController controller) : controller_(controller) {}
    void RunSession(boost::asio::ip::tcp::socket &&socket) {
        std::make_shared<Session>(std::move(socket), controller_)->Run();
    }
};
}  // namespace dtl
