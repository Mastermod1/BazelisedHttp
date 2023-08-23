cc_library(
    name = "http_server",
    srcs = ["http_server/httpServer.cpp"],
    hdrs = ["http_server/httpServer.hpp"],
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [
        ":http_server"
    ],
)
