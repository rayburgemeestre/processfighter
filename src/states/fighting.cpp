#include "states/fighting.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <sstream>
#include "states.h"
#include "messages.h"
#include "global_game_state.h"

#include <iterator>

extern int window_width;
extern int window_height;

fighting::fighting(global_game_state &gs)
  : state_interface(gs, game_state::state_type::fighting),
    heartbeat_(std::chrono::high_resolution_clock::now())
{
}

void fighting::initialize()
{
  messages::challenge_accepted ca(global_game_state_);
  global_game_state_.send_socket().send(ca.packet(), opponent_.ip, opponent_.port);
}

void fighting::set_opponent(probed_opponent_type opp)
{
  opponent_ = opp;
}

void fighting::set_lag(std::deque<double> &lag)
{
  lag_ = lag;
}

void fighting::handle(std::vector<std::unique_ptr<messages::message_interface>> msgs)
{
  for (auto &msg : msgs) {
    auto ping_msg = dynamic_cast<messages::ping *>(msg.get());
    if (ping_msg) {
      messages::pong p(global_game_state_, ping_msg->payload());
      global_game_state_.send_socket().send(p.packet(), opponent_.ip, opponent_.port);
    }
    auto pong_msg = dynamic_cast<messages::pong *>(msg.get());
    if (pong_msg) {
      std::stringstream ss;
      sf::Int64 payload_ = pong_msg->payload();
      auto transmitted_at = *reinterpret_cast<decltype(begin_)*>(&payload_);
      auto current_time = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> lag = current_time - transmitted_at;

      lag_.push_back(lag.count());
      if (lag_.size() > 5) {
        lag_.pop_front();
      }
      double lag_average = 0;
      for (double a : lag_) {
        lag_average += a;
      }
      lag_average /= lag_.size();
      ss << "lag = " << lag.count() << " avg = " << lag_average;
      global_game_state_.console_out().log(ss.str());
    }
  }
}

void fighting::tick()
{
  auto current_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed_total = current_time - begin_;
  if (elapsed_total.count() > 3000) {
    game_state state(state_);
    auto new_state = states::factory(state.transition(game_state::transition_type::next), global_game_state_);
    auto fighting_state = dynamic_cast<fighting *>(new_state.get());
    if (fighting_state) {
      fighting_state->set_opponent(opponent_);
      fighting_state->set_lag(lag_);
      next_state_ = std::move(new_state);
    }
  }
  std::chrono::duration<double, std::milli> elapsed_hb = current_time - heartbeat_;
  if (elapsed_hb.count() > 100) {
    if (sizeof(current_time) == sizeof(sf::Int64)) {
      // I don't care about reinterpreting, just need to serialize 8 bytes here..
      sf::Int64 *current_time_s = reinterpret_cast<sf::Int64 *>(&current_time);
      messages::ping p(global_game_state_, *current_time_s);
      global_game_state_.send_socket().send(p.packet(), opponent_.ip, opponent_.port);
      heartbeat_ = current_time;
    }
  }
  global_game_state_.my_ship().left(sf::Keyboard::isKeyPressed(sf::Keyboard::Left), current_time);
  global_game_state_.my_ship().right(sf::Keyboard::isKeyPressed(sf::Keyboard::Right), current_time);
}

void fighting::draw(sf::RenderTarget &renderTarget)
{
}
