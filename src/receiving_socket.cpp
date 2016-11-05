/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <iostream>
#include <sstream>
#include <messages.h>

#include "receiving_socket.h"
#include "global_game_state.h"

receiving_socket::receiving_socket(global_game_state &gs)
  : global_game_state_(gs)
{
}
receiving_socket::~receiving_socket() {
  running = false;

  messages::shutdown s(global_game_state_);
  global_game_state_.send_socket().send(s.packet(), "127.0.0.1", global_game_state_.my_udp_port());
  receiving_.join();
}


bool receiving_socket::bind(uint16_t port) {
  global_game_state_.recv_socket().setBlocking(true);
  if (global_game_state_.recv_socket().bind(port) != sf::Socket::Done) {
    std::cout << "Could not bind to UDP port " << port << std::endl;
    return false;
  }

  // non blocking udp socket gave me problems with Ubuntu, maybe it's the specific sfml version
  // as I don't feel like providing my own SFML with this simple game, I'll just wrap it in a std::thread
  //main_socket.setBlocking(false);

  receiving_ = std::thread([=]() {
    while (running) {
      sf::IpAddress ipaddr;
      unsigned short port;

      sf::Packet packet;
      sf::Int32 id = 0;
      if (global_game_state_.recv_socket().receive(packet, ipaddr, port) == sf::Socket::Done) {
        if (!(packet >> id)) {
          if (id)
            global_game_state_.console_out().log("DEBUG: error reading packet..");
          continue;
        }
        std::stringstream ss;
        ss << "Got packet: " << id << " " << "ipaddr = " << ipaddr << " port = " << port;
        switch (id) {
          case static_cast<sf::Int32>(messages::message_types::shutdown): {
            messages::shutdown s(global_game_state_);
            if (!s.parse(packet, ipaddr)) {
              continue;
            }
            ss << "shutdown";
            std::cout << ss.str() << std::endl;
            return;
          }
          case static_cast<sf::Int32>(messages::message_types::probe): {
            std::unique_ptr<messages::message_interface> m = std::make_unique<messages::probe> (global_game_state_);
            if (!m->parse(packet, ipaddr)) {
              continue;
            }
            receive(m);
            break;
          }
          case static_cast<sf::Int32>(messages::message_types::probe_response): {
            std::unique_ptr<messages::message_interface> m = std::make_unique<messages::probe_response> (global_game_state_);
            if (!m->parse(packet, ipaddr)) {
              continue;
            }
            receive(m);
            break;
          }
        }
        if (id) global_game_state_.console_out().log(ss.str());
        std::cout << ss.str() << std::endl;
      }
    }
  });
  return true;
}

void receiving_socket::receive(std::unique_ptr<messages::message_interface> &mt)
{
  std::lock_guard<std::mutex> lock(mut_);
  {
    messages_.emplace_back(std::move(mt));
  }
}

std::vector<std::unique_ptr<messages::message_interface>> receiving_socket::messages()
{
  std::lock_guard<std::mutex> lock(mut_);
  {
    std::vector<std::unique_ptr<messages::message_interface>> ret;
    std::swap(ret, messages_);
    return ret;
  }
}
