#include "t.h"

#include <cstdlib>

#include "envoy/buffer/buffer.h"
#include "envoy/http/header_map.h"
#include "envoy/network/connection.h"

#include "source/common/common/assert.h"
#include "source/extensions/filters/network/dubbo_proxy/app_exception.h"
#include "source/extensions/filters/network/dubbo_proxy/message_impl.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace DubboProxy {
namespace DubboFilters {
namespace Test {

Echo2::~Echo2() = default;

void Echo2::onDestroy() {}

void Echo2::setDecoderFilterCallbacks(DubboFilters::DecoderFilterCallbacks& callbacks) {
  callbacks_ = &callbacks;
}

void Echo2::onUpstreamData(Buffer::Instance& buffer, bool) {
  ENVOY_CONN_LOG(info, "upstream data: {}", *callbacks_->connection(), buffer.toString());
}

static int count = 0;

FilterStatus Echo2::onMessageDecoded(MessageMetadataSharedPtr p, ContextSharedPtr c) {
  ENVOY_CONN_LOG(info, "dubbo decode streamid: {}", *callbacks_->connection(),
                 callbacks_->streamId());

  ENVOY_CONN_LOG(info, "dubbo message decode, xxxxxxxxxxxxxxxxxxxxxx1xxxxxxx",
                 *callbacks_->connection());
  ENVOY_CONN_LOG(info, "dubbo method, {}", *callbacks_->connection(),
                 p->invocationInfo().methodName());
  const auto invocation = dynamic_cast<const DubboProxy::RpcInvocationImpl*>(&p->invocationInfo());

  ENVOY_CONN_LOG(info, "dubbo headers, {}", *callbacks_->connection(),
                 invocation->attachment().headers().size());
  auto call = callbacks_;
  invocation->attachment().headers().iterate(
      [call](const Http::HeaderEntry& e) -> Http::HeaderMap::Iterate {
        ENVOY_CONN_LOG(info, "dubbo header, {}: {}", *call->connection(), e.key().getStringView(),
                       e.value().getStringView());
        return Http::HeaderMap::Iterate::Continue;
      });
  // invocation->attachment().headers()
  ENVOY_CONN_LOG(info, "dubbo message, {}", *callbacks_->connection(),
                 c->originMessage().toString());
  count++;
  if (count % 3 == 0) {
    callbacks_->sendLocalReply(
        DubboProxy::AppException(ResponseStatus::ClientError,
                                 fmt::format("dubbo router: unknown cluster")),
        false);
    return FilterStatus::StopIteration;
  } else if (count % 3 == 1) {
    callbacks_->sendLocalReply(
        DubboProxy::AppException(ResponseStatus::ClientError,
                                 fmt::format("dubbo router: unknown cluster")),
        true);
    return FilterStatus::StopIteration;
  }
  return FilterStatus::Continue;
}
void Echo2::setEncoderFilterCallbacks(DubboFilters::EncoderFilterCallbacks& callbacks) {
  encoder_callbacks_ = &callbacks;
}

FilterStatus Echo2::onMessageEncoded(MessageMetadataSharedPtr, ContextSharedPtr c) {

  // ENVOY_CONN_LOG(info, "dubbo encode host, {}", *callbacks_->connection(),
  //                encoder_callbacks_->streamInfo().upstreamHost()->hostname());
  ENVOY_CONN_LOG(info, "dubbo encode message before, {}", *callbacks_->connection(),
                 c->originMessage().toString());
  // auto s = c->originMessage().toString().replace(30, 3, "okk");
  // c->originMessage().writeByte('\0');
  // c->originMessage().prepend(s);

  ENVOY_CONN_LOG(info, "dubbo encode message after, {}", *callbacks_->connection(),
                 c->originMessage().toString());
  // c->originMessage().add("x", 1);

  return FilterStatus::Continue;
}

} // namespace Test
} // namespace DubboFilters
} // namespace DubboProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
