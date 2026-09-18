#pragma once
#include "State.hpp"
#include "PCH.hpp"
// #include "Server.hpp"
class Server;
extern std::unique_ptr<sf::RenderWindow> window;
extern std::unique_ptr<Kosmic::State> state;
extern std::unique_ptr<Server> server;