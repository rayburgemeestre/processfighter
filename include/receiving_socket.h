/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <thread>
#include <mutex>

class global_game_state;

#include "messages.h"

class receiving_socket
{
public:
  receiving_socket(global_game_state &gs);
  ~receiving_socket();
  bool bind(uint16_t port);
  void receive(std::unique_ptr<messages::message_interface> &m);
  std::vector<std::unique_ptr<messages::message_interface>> messages();

private:
  global_game_state & global_game_state_;
  std::thread receiving_;
  volatile bool running = true;
  std::mutex mut_;
  std::vector<std::unique_ptr<messages::message_interface>> messages_;
};
