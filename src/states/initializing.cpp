#include "libs/tiny-process-library/process.hpp"
using process = Process;
#include "states/initializing.h"
#include "states/probing.h"
#include "states.h"

#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "global_game_state.h"
#include "utils/console.h"


initializing::initializing(global_game_state &gs)
  : state_interface(gs, game_state::state_type::initializing)
{
}

void initializing::initialize()
{
  global_game_state_.console_out().log("Running arp to discover possible players on the network");
  using namespace std;
  game_state state(state_);
  auto new_state = states::factory(state.transition(game_state::transition_type::next), global_game_state_);
  auto probing_state = dynamic_cast<probing *>(new_state.get());
  if (probing_state) {
    auto process_arp_line = [&](const char *bytes, size_t n) {
      stringstream ss(std::string(bytes, n));
      string item;
      while (getline(ss, item, '\n')) {
        stringstream ss(item);
        string name, ip;
        getline(ss, name, '\t');
        getline(ss, ip, '\t');
        probing_state->probe_opponent(opponent_type{0, name, ip, 0});
      }
    };
    process proc("arp -a | sed 's/\\(.*\\) (\\(.*\\)).*/\\1\\t\\2/'", "", process_arp_line);
    auto exit_status = proc.get_exit_status();
    cout << "process returned: " << exit_status << " (" << (exit_status == 0 ? "success" : "failure") << ")" << endl;
    if (exit_status != 0) {
      // TODO : implement
      std::cout << "Failure.. returning to initialize in 10 seconds.." << std::endl;
      return;
    }
    // for debugging purposes add local dev machine
    probing_state->probe_opponent(opponent_type{global_game_state_.unique_id(), "memyself", "10.75.99.6"});
    probing_state->probe_opponent(opponent_type{global_game_state_.unique_id(), "memyself2", "10.2.182.47"});
    next_state_ = std::move(new_state);
  }
}

void initializing::handle(std::vector<std::unique_ptr<messages::message_interface>> msgs)
{
}

void initializing::tick()
{
}

void initializing::draw(sf::RenderTarget &renderTarget)
{
}
