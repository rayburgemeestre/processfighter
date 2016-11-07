#include "states/countdown.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <sstream>
#include "utils/menu.h"
#include "states.h"
#include "messages.h"
#include "global_game_state.h"

#include <iterator>

extern int window_width;
extern int window_height;

countdown::countdown(global_game_state &gs)
  : state_interface(gs, game_state::state_type::countdown),
    heartbeat_(std::chrono::high_resolution_clock::now())
{
}

void countdown::initialize()
{
  messages::challenge_accepted ca(global_game_state_);
  global_game_state_.send_socket().send(ca.packet(), opponent_.ip, opponent_.port);
}

void countdown::set_opponent(probed_opponent_type opp)
{
  opponent_ = opp;
}

void countdown::handle(std::vector<std::unique_ptr<messages::message_interface>> msgs)
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

void countdown::tick()
{
  auto current_time = std::chrono::high_resolution_clock::now();
  // std::chrono::duration<double, std::milli> elapsed_total = current_time - begin_;
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
}

void countdown::draw(sf::RenderTarget &renderTarget)
{
}
