/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace algo {
namespace framework {

class Dispatcher {
 public:
  virtual uint32_t next_order_id() const = 0;

  virtual void operator()(CreateOrder const &) = 0;
  virtual void operator()(ModifyOrder const &) = 0;
  virtual void operator()(CancelOrder const &) = 0;
};

}  // namespace framework
}  // namespace algo
}  // namespace roq
