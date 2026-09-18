#include "Client.hpp"
#include "CSMessage.hpp"
#include "Server.hpp"
#include "Main.hpp"
Client::Client(sf::RenderTarget *target, Player player)
{
    this->renderTarget = target;
    this->player = player;
    otherPlayers = {};
}

void Client::DerivedUpdate()
{
    if (inputState.Down(sf::Keyboard::Key::S))
    {
        player.position += sf::Vector2f{0.f, speed * deltaTime};
        player.UpdateRect();
        auto msg = std::make_unique<CSMessageRequestSetPlayerPosition>();
        msg->requestId = GetNextReqestId();
        msg->newPosition = player.position;
        sf::Packet p;
        msg->ToPacket(p);
        SendPacket(p);
    }
    if (inputState.Down(sf::Keyboard::Key::W))
    {
        player.position += sf::Vector2f{0.f, -speed * deltaTime};
        player.UpdateRect();
        auto msg = std::make_unique<CSMessageRequestSetPlayerPosition>();
        msg->requestId = GetNextReqestId();
        msg->newPosition = player.position;
        sf::Packet p;
        msg->ToPacket(p);
        SendPacket(p);
    }
    if (inputState.Down(sf::Keyboard::Key::A))
    {
        player.position += sf::Vector2f{-speed * deltaTime, 0.f};
        player.UpdateRect();
        auto msg = std::make_unique<CSMessageRequestSetPlayerPosition>();
        msg->requestId = GetNextReqestId();
        msg->newPosition = player.position;
        sf::Packet p;
        msg->ToPacket(p);
        SendPacket(p);
    }
    if (inputState.Down(sf::Keyboard::Key::D))
    {
        player.position += sf::Vector2f{speed * deltaTime, 0.f};
        player.UpdateRect();
        auto msg = std::make_unique<CSMessageRequestSetPlayerPosition>();
        msg->requestId = GetNextReqestId();
        msg->newPosition = player.position;
        sf::Packet p;
        msg->ToPacket(p);
        SendPacket(p);
    }
}

void Client::DerivedRender()
{
    renderTarget->draw(player.rect);
    for (auto &p : otherPlayers)
    {
        renderTarget->draw(p.rect);
    }
}

void Client::ProcessPacket(sf::Packet &packet)
{
    uint8_t packetType;
    packet >> packetType;
    CSMessage *msg = CSMessageFromType((CSMessageType)packetType);
    msg->FromPacket(packet);
    if (msg->type == CSMessageType::SET_PLAYER_POSITION)
    {
        CSMessageSetPlayerPosition *msg2 = (CSMessageSetPlayerPosition *)msg;
        for (auto &p : otherPlayers)
        {
            if (p.id == msg2->playerId)
            {
                p.position = msg2->newPosition;
            }
        }
    }
    else if (msg->type == CSMessageType::REQUEST_STATUS)
    {
        CSMessageRequestStatus *msg2 = (CSMessageRequestStatus *)msg;
        for (int i = 0; i < requests.size(); i++)
        {
            if (requests[i]->requestId == msg2->requestId)
            {
                if (requests[i]->type == CSMessageType::SET_PLAYER_POSITION)
                {
                    // nothing to do
                }
            }
        }
    }
    else if (msg->type == CSMessageType::JOIN_DATA)
    {
        LoadJoinData(packet);
    }
    delete msg;
}

void Client::SendPacket(sf::Packet &packet)
{
    if (server.get() != nullptr)
    {
        server->HandlePacket(player.id,packet);
        return;
    }
    if (!connected)
        return;

    sf::Socket::Status status = socket.send(packet);

    if (status == sf::Socket::Status::Disconnected)
    {
        connected = false;
    }
    // TODO: check if server isnt null, if so then send directly to server
    // otherwise use socket to send to server through network
}

void Client::LoadJoinData(sf::Packet &packet)
{
    // first load client player's data
    packet >> player.id >> player.color.r >> player.color.g >> player.color.b;
    packet >> player.position.x >> player.position.y;
    player.UpdateRect();
    int numPlayers;
    packet >> numPlayers;
    for (int i = 0; i < numPlayers; i++)
    {
        Player player;
        packet >> player.id >> player.color.r >> player.color.g >> player.color.b;
        packet >> player.position.x >> player.position.y;
        otherPlayers.push_back(player);
    }
}

bool Client::ConnectToServer(sf::IpAddress &ip, unsigned short port)
{
    // If we're already connected, don't reconnect.
    if (connected)
        return true;

    // Temporarily make the socket blocking so connect() can
    // actually wait for the connection to be established.
    socket.setBlocking(true);

    if (socket.connect(ip, port) != sf::Socket::Status::Done)
    {
        socket.setBlocking(false);
        connected = false;
        return false;
    }

    socket.setBlocking(false);
    connected = true;

    return true;
}


void Client::ReceivePackets()
{
    if (!connected)
        return;

    while (true)
    {
        sf::Packet packet;

        sf::Socket::Status status = socket.receive(packet);

        if (status == sf::Socket::Status::Done)
        {
            ProcessPacket(packet);
        }
        else if (status == sf::Socket::Status::NotReady)
        {
            // No more packets currently waiting.
            break;
        }
        else if (status == sf::Socket::Status::Disconnected)
        {
            connected = false;
            break;
        }
        else
        {
            // Error
            connected = false;
            break;
        }
    }
}