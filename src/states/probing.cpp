#include "states/probing.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <sstream>
#include "utils/console.h"

extern sf::UdpSocket main_sending_socket;
extern console out;

probing::probing()
  : state_interface(game_state::state_type::probing)
{
}

void probing::initialize()
{
  for (const auto &opponent : opponents_) {
    std::stringstream ss;
    ss << "Probing " << opponent.ip << " (" << opponent.name << ")";
    out.log(ss.str());

    sf::Packet packet;
    sf::Int32 id = 1000;
    std::string s = "Hello world..";
    packet << id << s;
    main_sending_socket.send(packet, opponent.ip, 7777);
    out.log("sending UDP packet!");

    sf::Packet packet2;
    sf::Int32 id2 = 1001;
    float test1 = 1001.1001;
    float test2 = 1111.1111;
    std::string test3 = "Thank you";
    packet2 << id2 << test1 << test2 << test3;
    main_sending_socket.send(packet2, opponent.ip, 7777);
  }
}

void probing::probe_opponent(opponent_type type)
{
  opponents_.push_back(type);
}

void probing::draw(sf::RenderTarget &renderTarget)
{
}
