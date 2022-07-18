#pragma once

#include <memory>

#include "dtl/iocontext.h"
#include "dtl/listener.h"

namespace dtl {
template <class Server>
concept ServerConcept = requires(Server server, IOContext io_context,
                                 std::shared_ptr<Listener> listener) {
    Server(io_context, listener);
};
}  // namespace dtl
