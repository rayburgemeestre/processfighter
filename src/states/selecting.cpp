#include "states/selecting.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <chrono>
#include <sstream>
#include "global_game_state.h"
#include "utils/menu.h"
#include "states.h"
#include "states/initializing.h"


extern int window_width;
extern int window_height;

selecting::selecting(global_game_state &gs)
  : state_interface(gs, game_state::state_type::selecting)
{
}

void selecting::initialize()
{
}

void selecting::add_opponent(probed_opponent_type type)
{
  opponents_.push_back(type);
}

void selecting::handle(std::vector<std::unique_ptr<messages::message_interface>> msgs)
{
  for (auto &msg : msgs) {
    auto probe_msg = dynamic_cast<messages::probe *>(msg.get());
    if (probe_msg) {
      if (probe_msg->requesting_id() != global_game_state_.unique_id()) {
        messages::probe_response r(global_game_state_);
        global_game_state_.send_socket().send(r.packet(), probe_msg->ipaddr(), probe_msg->source_port());
        auto opponent_known = [&](size_t id) {
          for (const auto &opp : opponents_) {
            if (opp.id == id) {
              return true;
            }
          }
          return false;
        };
        if (!opponent_known(probe_msg->requesting_id())) {
          messages::probe p(global_game_state_);
          global_game_state_.send_socket().send(p.packet(), probe_msg->ipaddr(), probe_msg->source_port());
        }
      }
    }
    auto probe_response_msg = dynamic_cast<messages::probe_response *>(msg.get());
    if (probe_response_msg) {
      opponents_.push_back(probed_opponent_type{probe_response_msg->requesting_id(),
                                                probe_response_msg->name(),
                                                probe_response_msg->ipaddr().toString(),
                                                probed_opponent_type::probed_opponent_state::idle});
    }
  }
}

void selecting::tick()
{
}

void selecting::draw(sf::RenderTarget &renderTarget)
{
  menu m(window_width, window_height);
  m.add_item(std::string("Reinitialize list"), [&](){
    game_state state(state_);
    auto new_state = states::factory(state.transition(game_state::transition_type::refresh), global_game_state_);
    next_state_ = std::move(new_state);
  });
  for (auto &opp: opponents_) {
    std::stringstream ss;
    ss << "Challenge: " << opp.name << " (" << opp.ip << ")";
    auto str = ss.str();
    m.add_item(str, [&, opp](){
      std::cout << "You have selected: " << opp.id << " " << opp.name << " " << opp.ip << std::endl;
      std::cout << "Waiting for his accept/decline..." << std::endl;
      // TODO: go to waiting state and challenge opponent with packet in initialize()
    });
  }
  m.add_item("Exit", [](){
    std::exit(0);
  });
  m.render(renderTarget);
}
