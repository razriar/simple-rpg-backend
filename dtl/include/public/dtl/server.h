#pragma once

#include "iocontext.h"
#include "listener.h"

namespace dtl {
class Server {
   private:
    IOContext io_context_;
    std::shared_ptr<Listener> listener_;

   public:
    Server(IOContext &io_context, std::shared_ptr<Listener> listener)
        : io_context_(std::move(io_context)), listener_(listener) {}
    void Run() {
        listener_->Run();
        io_context_.Run();
    }
};
}  // namespace dtl
