/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "global_game_state.h"
#include <iostream>
#include <random>
#include <limits>

global_game_state::global_game_state(std::string name)
   : console_out_(5), rs(*this), name_(name)
{
  uint16_t port = 7777;
  if (rs.bind(port)) {
    my_udp_port_ = port;
  }
  else if (rs.bind(++port)) {
    my_udp_port_ = port;
  }
  else if (rs.bind(++port)) {
    my_udp_port_ = port;
  }
  else {
    std::cout << "Could not bind to either port 7777-7779" << std::endl;
    std::exit(1);
  }
  std::cout << "Bound to UDP port " << my_udp_port_ << std::endl;
}

size_t global_game_state::my_udp_port()
{
  return my_udp_port_;
}

sf::Int64 global_game_state::unique_id()
{
  if (unique_id_ == 0) {
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<size_t> uniform_dist(
      std::numeric_limits<size_t>::min(),
      std::numeric_limits<size_t>::max()
    );
    unique_id_ = uniform_dist(e1);
    std::cout << "Randomly-chosen Unique ID: " << unique_id_ << '\n';
  }
  return unique_id_;
}

console & global_game_state::console_out()
{
  return console_out_;
}

sf::UdpSocket &global_game_state::send_socket()
{
  return main_sending_socket_;
}

sf::UdpSocket &global_game_state::recv_socket()
{
  return main_socket_;
}

std::string global_game_state::name()
{
  return name_;
}
