#include "states/probing.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <sstream>
#include "global_game_state.h"
#include "messages.h"
#include "states.h"
#include "states/selecting.h"

probing::probing(global_game_state &gs)
  : state_interface(gs, game_state::state_type::probing)
{

}

void probing::initialize()
{
  for (const auto &opponent : opponents_) {
    std::stringstream ss;
    ss << "Probing " << opponent.ip << " (" << opponent.name << ")";
    global_game_state_.console_out().log(ss.str());

    messages::probe probe(global_game_state_);

    global_game_state_.send_socket().send(probe.packet(), opponent.ip, 7777);
    global_game_state_.send_socket().send(probe.packet(), opponent.ip, 7778);
    global_game_state_.send_socket().send(probe.packet(), opponent.ip, 7779);
  }
}

void probing::probe_opponent(opponent_type type)
{
  opponents_.push_back(type);
}

void probing::probed_opponent(opponent_type type)
{
  opponents_probed_.push_back(type);
}

void probing::handle(std::vector<std::unique_ptr<messages::message_interface>> msgs)
{
  for (auto &msg : msgs) {
    auto probe_msg = dynamic_cast<messages::probe *>(msg.get());
    if (probe_msg) {
      if (probe_msg->requesting_id() != global_game_state_.unique_id()) {
        messages::probe_response r(global_game_state_);
        global_game_state_.send_socket().send(r.packet(), probe_msg->ipaddr(), probe_msg->source_port());
      }
    }
    auto probe_response_msg = dynamic_cast<messages::probe_response *>(msg.get());
    if (probe_response_msg) {
      opponents_probed_.push_back(opponent_type{probe_response_msg->requesting_id(),
                                                probe_response_msg->name(),
                                                probe_response_msg->ipaddr().toString()});
    }
  }
}

void probing::tick()
{
  auto current_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = current_time - begin_;

  if (elapsed.count() > 2000) {
    game_state state(state_);
    auto new_state = states::factory(state.transition(game_state::transition_type::next), global_game_state_);
    auto selecting_state = dynamic_cast<selecting *>(new_state.get());
    if (selecting_state) {
      for (auto &opponent : opponents_probed_) {
        selecting_state->add_opponent(probed_opponent_type{opponent.id,
                                                           opponent.name,
                                                           opponent.ip,
                                                           probed_opponent_type::probed_opponent_state::idle});
      }
    }
    next_state_ = std::move(new_state);
  }
}

void probing::draw(sf::RenderTarget &renderTarget)
{
}
