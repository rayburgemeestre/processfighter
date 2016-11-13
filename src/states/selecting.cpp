#include "states/selecting.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <chrono>
#include <sstream>
#include "global_game_state.h"
#include "utils/menu.h"
#include "states.h"
#include "states/waiting.h"
#include "states/challenged.h"


extern int window_width;
extern int window_height;

selecting::selecting(global_game_state &gs)
  : state_interface(gs, game_state::state_type::selecting), m(window_width, window_height)
{
}

void selecting::initialize()
{
  m.add_item(std::string("Reinitialize list"), [&](){
    game_state state(state_);
    auto new_state = states::factory(state.transition(game_state::transition_type::refresh), global_game_state_);
    next_state_ = std::move(new_state);
  });

  m.add_item(std::string("Challenge: yourself"), [&](){
    global_game_state_.console_out().log("You cannot challenge yourself!");
  });

  for (auto &opp: opponents_) {
    std::stringstream ss;
    ss << "Challenge: " << opp.name << " (" << opp.ip << ")";
    auto str = ss.str();
    m.add_item(str, [&, opp](){
      std::cout << "You have selected: " << opp.id << " " << opp.name << " " << opp.ip << std::endl;
      game_state state(state_);
      auto new_state = states::factory(state.transition(game_state::transition_type::select), global_game_state_);
      auto waiting_state = dynamic_cast<waiting *>(new_state.get());
      if (waiting_state) {
        waiting_state->set_opponent(opp);
        next_state_ = std::move(new_state);
      }
    });
  }
  m.add_item("Exit", [](){
    std::exit(0);
  });
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
                                                probe_response_msg->source_port(),
                                                probed_opponent_type::probed_opponent_state::idle});
    }
    auto challenge_msg = dynamic_cast<messages::challenge *>(msg.get());
    if (challenge_msg) {
      probed_opponent_type opponent{challenge_msg->requesting_id(),
                                    challenge_msg->name(),
                                    challenge_msg->ipaddr().toString(),
                                    challenge_msg->source_port(),
                                    probed_opponent_type::probed_opponent_state::idle};
      global_game_state_.console_out().log("I am challenged by " + opponent.name);
      game_state state(state_);
      auto new_state = states::factory(state.transition(game_state::transition_type::challenge), global_game_state_);
      auto challenged_state = dynamic_cast<challenged *>(new_state.get());
      if (challenged_state) {
        challenged_state->set_opponent(opponent);
        next_state_ = std::move(new_state);
      }
    }
  }
}

void selecting::tick()
{
}

// While developing...

#include <chrono>
void selecting::draw(sf::RenderTarget &renderTarget)
{
  m.render(renderTarget);
  auto current_time = std::chrono::high_resolution_clock::now();
  global_game_state_.my_ship().left(sf::Keyboard::isKeyPressed(sf::Keyboard::Left), current_time);
  global_game_state_.my_ship().right(sf::Keyboard::isKeyPressed(sf::Keyboard::Right), current_time);
  global_game_state_.my_ship().fire(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space), current_time);
}
