#pragma once
#include "PCH.hpp"

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
        AcceptClients();
        ReceivePackets();
    }

    // Send a packet to one specific client
    void SendToClient(std::size_t clientID,sf::Packet& packet)
    {
        if (clientID >= clients.size())
            return;

        clients[clientID].socket.send(packet);
    }

    // Send a packet to every connected client
    void Broadcast(sf::Packet& packet)
    {
        for (auto& client : clients)
        {
            client.socket.send(packet);
        }
    }

private:

    struct Client
    {
        sf::TcpSocket socket;
        std::size_t id;
    };

    sf::TcpListener listener;
    std::vector<Client> clients;

    void AcceptClients()
    {
        while (true)
        {
            Client client;

            sf::Socket::Status status = listener.accept(client.socket);

            if (status == sf::Socket::Status::NotReady)
                break;

            if (status != sf::Socket::Status::Done)
                continue;

            client.id = clients.size();

            client.socket.setBlocking(false);

            std::cout << "Client connected: " << client.id << '\n';

            clients.push_back(std::move(client));

            // Tell the new client its ID
            sf::Packet packet;
            packet << static_cast<std::uint32_t>(clients.back().id);

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
                HandlePacket(i, packet);

                ++i;
            }
            else if (status == sf::Socket::Status::NotReady)
            {
                ++i;
            }
            else
            {
                // Disconnected / error
                std::cout << "Client disconnected: "
                          << clients[i].id << '\n';

                clients.erase(clients.begin() + i);
            }
        }
    }

    void HandlePacket(std::size_t senderID, sf::Packet& packet)
    {
        std::uint8_t type;
        packet >> type;

        switch (type)
        {
            case 0:
                HandleChat(senderID, packet);
                break;

            case 1:
                HandlePlayerMovement(senderID, packet);
                break;

            case 2:
                HandleSomethingElse(senderID, packet);
                break;
        }
    }

    void HandleChat(std::size_t senderID, sf::Packet& packet)
    {
        std::string message;
        packet >> message;

        std::cout << "Client " << senderID
                  << " says: " << message << '\n';

        // Example: broadcast the message to everyone
        sf::Packet outgoing;

        outgoing << static_cast<std::uint8_t>(0);
        outgoing << static_cast<std::uint32_t>(senderID);
        outgoing << message;

        Broadcast(outgoing);
    }

    void HandlePlayerMovement(std::size_t senderID, sf::Packet& packet)
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

    void HandleSomethingElse(std::size_t senderID, sf::Packet& packet)
    {
        // ...
    }
};