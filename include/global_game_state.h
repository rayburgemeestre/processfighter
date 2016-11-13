/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "SFML/Network.hpp"
#include "utils/console.h"
#include "receiving_socket.h"
#include "utils/ship.h"

class global_game_state
{
public:
  console & console_out();
  sf::UdpSocket &send_socket();
  sf::UdpSocket &recv_socket();
  size_t my_udp_port();
  sf::Int64 unique_id();
  std::string name();

  ship & my_ship();
  ship & enemy_ship();

protected:
  global_game_state(std::string name);
  receiving_socket rs;

// state of interest to others...
  size_t unique_id_ = 0;
  size_t my_udp_port_ = 7777;
  sf::UdpSocket main_socket_;
  sf::UdpSocket main_sending_socket_;
  console console_out_;

  sf::Sprite &ship_sprite();
  sf::Sprite &enemy_ship_sprite();

private:
  std::string name_;

  ship ship_;
  ship enemy_ship_;
};
