#include <string>

#include "t.h"

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"
#include "source/extensions/filters/network/dubbo_proxy/filters/filter_config.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace DubboProxy {
namespace DubboFilters {
namespace Test {

/**
 * Config registration for the echo2 filter. @see NamedNetworkFilterConfigFactory.
 */
class Echo2ConfigFactory : public NamedDubboFilterConfigFactory {
public:
  DubboFilters::FilterFactoryCb createFilterFactoryFromProto(const Protobuf::Message&,
                                                        const std::string&, Server::Configuration::FactoryContext& context) override {
                                                            
    return [&context](DubboFilters::FilterChainFactoryCallbacks& callbacks) -> void {
        // callbacks.addFilter(DubboFilters::CodecFilterSharedPtr(std::make_shared<Echo2>(context.clusterManager())));
        callbacks.addFilter(std::make_shared<Echo2>(context.clusterManager()));
    };
  }

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new Envoy::ProtobufWkt::Struct()};
  }

  std::string name() const override { return "envoy.filters.dubbo.echo2"; }

};

/**
 * Static registration for the echo2 filter. @see RegisterFactory.
 */
// static Registry::RegisterFactory<Echo2ConfigFactory, NamedDubboFilterConfigFactory> registered_;
REGISTER_FACTORY(Echo2ConfigFactory, DubboFilters::NamedDubboFilterConfigFactory);

} // namespace Configuration
} // namespace Server
} // namespace Envoy
}
}
}
