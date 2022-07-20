#include <cstdlib>
#include <iostream>

#include "dtl/echoaction.h"
#include "dtl/frontcontroller.h"
#include "dtl/serverfactory.h"

int main() {
    auto controller = dtl::FrontController();
    controller.RegisterAction<dtl::EchoAction>();
    auto factory = dtl::ServerFactory();
    auto server = factory.Create<dtl::Server>(controller, "8000", "4");
    server.Run();
    return EXIT_SUCCESS;
}
