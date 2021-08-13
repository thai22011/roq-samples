/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-3/application.h"

#include <cassert>
#include <chrono>
#include <memory>
#include <vector>

#include "roq/client.h"
#include "roq/exceptions.h"

#include "roq/samples/example-3/config.h"
#include "roq/samples/example-3/flags.h"
#include "roq/samples/example-3/strategy.h"

using namespace std::chrono_literals;
using namespace roq::literals;

namespace roq {
namespace samples {
namespace example_3 {

int Application::main_helper(const roq::span<std::string_view> &args) {
  assert(!args.empty());
  if (args.size() == 1)
    log::fatal("Expected arguments"_sv);
  if (args.size() != 2)
    log::fatal("Expected exactly one argument"_sv);
  Config config;
  // note!
  //   absl::flags will have removed all flags and we're left with arguments
  //   arguments can be a list of either
  //   * unix domain sockets (trading) or
  //   * event logs (simulation)
  auto connections = args.subspan(1);
  if (Flags::simulation()) {
    // collector
    auto snapshot_frequency = 1s;
    auto collector = client::detail::SimulationFactory::create_collector(snapshot_frequency);
    // matcher
    auto market_data_latency = 1ms;
    auto order_manager_latency = 1ms;
    // ... demonstrating how one could possibly set up multiple matchers (one for each exchange)
    std::vector<std::unique_ptr<Matcher> > matchers;
    matchers.emplace_back(client::detail::SimulationFactory::create_matcher(
        "simple"_sv, 0, Flags::exchange(), market_data_latency, order_manager_latency));
    // simulator
    client::Simulator(config, connections, *collector, matchers).dispatch<Strategy>();
  } else {
    // trader
    client::Trader(config, connections).dispatch<Strategy>();
  }
  return EXIT_SUCCESS;
}

int Application::main(int argc, char **argv) {
  // wrap arguments (prefer to not work with raw pointers)
  std::vector<std::string_view> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i)
    args.emplace_back(argv[i]);
  return main_helper(args);
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
