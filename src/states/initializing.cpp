#include "libs/tiny-process-library/process.hpp"
using process = Process;
#include "states/initializing.h"
#include "states/probing.h"
#include "states.h"

#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "utils/console.h"

extern console out;

initializing::initializing()
  : state_interface(game_state::state_type::initializing)
{
}

void initializing::initialize()
{
  out.log("Running arp to discover possible players on the network");
  using namespace std;
  game_state state(state_);
  auto new_state = states::factory(state.transition(game_state::transition_type::next));
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
        probing_state->probe_opponent(opponent_type{name, ip});
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
    probing_state->probe_opponent(opponent_type{"memyself", "10.75.99.6"});
    next_state_ = std::move(new_state);
  }
}

void initializing::draw(sf::RenderTarget &renderTarget)
{
}
