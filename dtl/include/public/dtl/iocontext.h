#pragma once

#include <boost/asio/io_context.hpp>
#include <thread>
#include <vector>

namespace dtl {
class IOContext {
   private:
    std::shared_ptr<boost::asio::io_context> io_context_;
    unsigned n_threads_;

   public:
    IOContext(IOContext&& other)
        : io_context_(other.io_context_), n_threads_(other.n_threads_) {}
    IOContext(IOContext& other)
        : io_context_(other.io_context_), n_threads_(other.n_threads_) {}
    IOContext(unsigned n_threads) : n_threads_(n_threads) {
        io_context_ = std::make_shared<boost::asio::io_context>(n_threads_);
    }
    void Run() {
        std::vector<std::thread> threads(n_threads_ - 1);
        for (int i = 0; i < n_threads_ - 1; i++)
            threads.emplace_back([&] { io_context_->run(); });
        io_context_->run();
    }
    operator boost::asio::io_context&() { return *io_context_; }
    boost::asio::io_context& data() { return *io_context_; }
};
}  // namespace dtl
