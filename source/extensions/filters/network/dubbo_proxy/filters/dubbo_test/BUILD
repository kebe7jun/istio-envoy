load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_extension_package",
)

licenses(["notice"])  # Apache 2

envoy_extension_package()

envoy_cc_library(
    name = "test",
    srcs = ["t.cc"],
    hdrs = ["t.h"],
    deps = [
        "//envoy/buffer:buffer_interface",
        "//envoy/http:header_map_interface",
        "//envoy/network:connection_interface",
        "//envoy/network:filter_interface",
        "//envoy/upstream:cluster_manager_interface",
        "//source/common/common:assert_lib",
        "//source/common/common:logger_lib",
        "//source/extensions/filters/network/dubbo_proxy:app_exception_lib",
        "//source/extensions/filters/network/dubbo_proxy:message_lib",
        "//source/extensions/filters/network/dubbo_proxy/filters:filter_interface",
    ],
)

envoy_cc_library(
    name = "config",
    srcs = ["test_config.cc"],
    deps = [
        ":test",
        "//envoy/registry",
        "//source/extensions/filters/network/dubbo_proxy/filters:factory_base_lib",
        "//source/extensions/filters/network/dubbo_proxy/filters:filter_config_interface",
    ],
)
