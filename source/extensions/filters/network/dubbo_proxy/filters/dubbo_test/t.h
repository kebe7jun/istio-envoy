#pragma once

#include "envoy/network/filter.h"

#include "source/common/common/logger.h"
#include "envoy/upstream/cluster_manager.h"
#include "source/extensions/filters/network/dubbo_proxy/filters/filter.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace DubboProxy {
namespace DubboFilters {
namespace Test {

/**
 * Implementation of a basic echo filter.
 */
class Echo2 : public DubboFilters::CodecFilter, Logger::Loggable<Logger::Id::dubbo> {
public:
  Echo2(Upstream::ClusterManager& cluster_manager) : cluster_manager_(cluster_manager){};
  ~Echo2() override;

  // Network::ReadFilter
  //   Network::FilterStatus onData(Buffer::Instance& data, bool end_stream) override;
  //   Network::FilterStatus onNewConnection() override { return Network::FilterStatus::Continue; }
  //   void initializeReadFilterCallbacks(Network::ReadFilterCallbacks& callbacks) override {
  //     read_callbacks_ = &callbacks;
  //   }
  void onDestroy() override;
  void setDecoderFilterCallbacks(DubboFilters::DecoderFilterCallbacks& callbacks) override;

  FilterStatus onMessageDecoded(MessageMetadataSharedPtr metadata, ContextSharedPtr ctx) override;

  // DubboFilter::EncoderFilter
  void setEncoderFilterCallbacks(DubboFilters::EncoderFilterCallbacks& callbacks) override;
  FilterStatus onMessageEncoded(MessageMetadataSharedPtr metadata, ContextSharedPtr ctx) override;
    // Tcp::ConnectionPool::UpstreamCallbacks
  // void onUpstreamData(Buffer::Instance& data, bool end_stream) override;
  // void onEvent(Network::ConnectionEvent event) override;
  // void onAboveWriteBufferHighWatermark() override {}
  // void onBelowWriteBufferLowWatermark() override {}

  // Network::FilterStatus onData(Buffer::Instance& data, bool end_stream) override;
  // Network::FilterStatus onNewConnection() override { return Network::FilterStatus::Continue; }
  // void initializeReadFilterCallbacks(Network::ReadFilterCallbacks& callbacks) override {
  //   read_callbacks_ = &callbacks;
  // }
private:
  Network::ReadFilterCallbacks* read_callbacks_{};
  //   Server::Configuration::FactoryContext& context_;
  DubboFilters::DecoderFilterCallbacks* callbacks_{};
  DubboFilters::EncoderFilterCallbacks* encoder_callbacks_{};
  Upstream::ClusterManager& cluster_manager_;
};

} // namespace Test
} // namespace DubboFilters
} // namespace DubboProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy