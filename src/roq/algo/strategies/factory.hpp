/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include <span>
#include <string>
#include <string_view>

#include "roq/cache/gateway.hpp"

#include "roq/algo/framework/dispatcher.hpp"
#include "roq/algo/framework/factory.hpp"
#include "roq/algo/framework/handler.hpp"
#include "roq/algo/framework/instrument.hpp"
#include "roq/algo/framework/market.hpp"
#include "roq/algo/framework/state.hpp"

namespace roq {
namespace algo {
namespace strategies {

class Factory final : public framework::Factory {
 public:
  Factory(
      std::string_view const &type,
      std::span<const framework::Instrument> const &,
      std::span<framework::Market const *> const &,
      std::span<const cache::Gateway> const &);

  Factory(Factory &&) = default;
  Factory(Factory const &) = delete;

  operator framework::State const &() const override { return state_; }

  std::unique_ptr<framework::Handler> create(
      framework::Dispatcher &, std::string_view const &routing_id, CreateOrder const &) override;

 private:
  std::string const type_;
  framework::State const state_;
};

}  // namespace strategies
}  // namespace algo
}  // namespace roq
