#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <string>

#include "concepts/serverconcept.h"
#include "frontcontroller.h"
#include "iocontext.h"
#include "listener.h"
#include "server.h"
#include "sessionfactory.h"

namespace dtl {
class ServerFactory {
   public:
    template <ServerConcept Server>
    Server Create(FrontController controller, const std::string &port_str,
                  const std::string &n_threads_str) {
        namespace asio = boost::asio;
        namespace ip = asio::ip;
        auto n_threads = std::max(1, std::atoi(n_threads_str.c_str()));
        auto io_context = IOContext(n_threads);
        auto acceptor_strand = asio::make_strand(io_context.data());
        auto port = static_cast<unsigned short>(std::atoi(port_str.c_str()));
        auto endpoint = ip::tcp::endpoint(ip::tcp::v4(), port);
        auto acceptor = ip::tcp::acceptor(acceptor_strand, endpoint);
        auto factory = SessionFactory(controller);
        auto listener =
            std::make_shared<Listener>(io_context, acceptor, factory);
        auto server = Server(io_context, listener);
        return server;
    }
};
}  // namespace dtl
