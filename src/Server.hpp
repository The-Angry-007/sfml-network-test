#pragma once
#include "PCH.hpp"
#include "Player.hpp"
#include "CSMessage.hpp"
#include "Main.hpp"
#include "Client.hpp"
class Server
{
public:
    bool Start(unsigned short port)
    {
        if (listener.listen(port) != sf::Socket::Status::Done)
        {
            std::cerr << "Failed to listen on port " << port << '\n';
            return false;
        }

        listener.setBlocking(false);

        std::cout << "Server listening on port " << port << '\n';
        return true;
    }

    void Update()
    {
        AcceptServerClients();
        ReceivePackets();
    }

    // Send a packet to one specific client
    void SendToServerClient(uint64_t clientID, sf::Packet &packet)
    {
        // if (clientID >= clients.size())
        // return;
        if (clientID == ((Client*)state.get())->player.id)
        {
            ((Client*)state.get())->ProcessPacket(packet);
            return;
        }
        for (auto &c : clients)
        {
            if (c.id == clientID)
            {
                c.socket.send(packet);
            }
        }
    }

    // Send a packet to every connected client
    void Broadcast(sf::Packet &packet, std::vector<uint64_t> exclusions = {})
    {
        for (auto &client : clients)
        {
            if (std::find(exclusions.begin(), exclusions.end(), client.id) == exclusions.end())
            {
                if (client.id == ((Client*)state.get())->player.id)
                {
                    ((Client*)state.get())->ProcessPacket(packet);
                    continue;;
                }
                client.socket.send(packet);
            }
        }
    }

public:
    struct ServerClient
    {
        sf::TcpSocket socket;
        std::uint64_t id;
    };

    sf::TcpListener listener;
    std::vector<ServerClient> clients;
    std::vector<Player> players = {};
    uint64_t GetNextPlayerId()
    {
        static uint64_t currId = 0;
        return currId++;
    }
    // adds a player then bundles all the current game data into a packet
    void AddNewPlayer(sf::Packet &packet)
    {
        Player p;
        p.id = GetNextPlayerId();
        p.color = sf::Color(rand() % 255, rand() % 255, rand() % 255);
        p.position = {rand() % 300, rand() % 300};
        packet << p.id << p.color.r << p.color.g << p.color.b;
        packet << p.position.x << p.position.y;
        packet << players.size();
        for (auto &p : players)
        {
            packet << p.id << p.color.r << p.color.g << p.color.b;
            packet << p.position.x << p.position.y;
        }
        players.push_back(p);
    }
    void AcceptServerClients()
    {
        while (true)
        {
            ServerClient client;

            sf::Socket::Status status = listener.accept(client.socket);

            if (status == sf::Socket::Status::NotReady)
                break;

            if (status != sf::Socket::Status::Done)
                continue;

            // client.id = clients.size();

            client.socket.setBlocking(false);

            clients.push_back(std::move(client));
            sf::Packet packet;
            packet << (uint8_t)CSMessageType::JOIN_DATA;
            AddNewPlayer(packet);
            clients.back().id = players.back().id;
            std::cout << "ServerClient connected: " << client.id << '\n';
            // Tell the new client its ID
            // packet << static_cast<std::uint32_t>(clients.back().id);

            clients.back().socket.send(packet);
        }
    }

    void ReceivePackets()
    {
        for (std::size_t i = 0; i < clients.size();)
        {
            sf::Packet packet;

            sf::Socket::Status status = clients[i].socket.receive(packet);

            if (status == sf::Socket::Status::Done)
            {
                HandlePacket(clients[i].id, packet);

                ++i;
            }
            else if (status == sf::Socket::Status::NotReady)
            {
                ++i;
            }
            else
            {
                // Disconnected / error
                std::cout << "ServerClient disconnected: "
                          << clients[i].id << '\n';

                clients.erase(clients.begin() + i);
            }
        }
    }

    void HandlePacket(std::uint64_t senderID, sf::Packet &packet)
    {
        std::cout << "Handling packet" << std::endl;
        std::uint8_t packetType;
        packet >> packetType;
        CSMessageType type = (CSMessageType)packetType;
        CSMessage *msg = CSMessageFromType(type);
        msg->FromPacket(packet);
        if (type == CSMessageType::REQUEST_SET_PLAYER_POSITION)
        {
            auto *msg2 = (CSMessageRequestSetPlayerPosition *)msg;
            for (auto &p : players)
            {
                if (p.id == senderID)
                {
                    p.position = msg2->newPosition;
                }
            }
            auto *outgoing = new CSMessageSetPlayerPosition();
            outgoing->playerId = senderID;
            outgoing->newPosition = msg2->newPosition;
            sf::Packet packet;
            outgoing->ToPacket(packet);
            Broadcast(packet, {senderID});
            delete outgoing;
        }
        delete msg;
    }
    

    void HandleChat(std::size_t senderID, sf::Packet &packet)
    {
        std::string message;
        packet >> message;

        std::cout << "ServerClient " << senderID
                  << " says: " << message << '\n';

        // Example: broadcast the message to everyone
        sf::Packet outgoing;

        outgoing << static_cast<std::uint8_t>(0);
        outgoing << static_cast<std::uint32_t>(senderID);
        outgoing << message;

        Broadcast(outgoing);
    }

    void HandlePlayerMovement(std::size_t senderID, sf::Packet &packet)
    {
        float x;
        float y;

        packet >> x >> y;

        // Do server-side processing here...

        // Then send the resulting state to everyone
        sf::Packet outgoing;

        outgoing << static_cast<std::uint8_t>(1);
        outgoing << static_cast<std::uint32_t>(senderID);
        outgoing << x << y;

        Broadcast(outgoing);
    }

    void HandleSomethingElse(std::size_t senderID, sf::Packet &packet)
    {
        // ...
    }
};