#pragma once
#include "PCH.hpp"
#include "Player.hpp"
enum class CSMessageType : uint8_t
{
    // REQUEST_JOIN_DATA = 0,       // client requests the info required upon joining
    REQUEST_SET_PLAYER_POSITION = 0, // client requests to update their player's position

    SET_PLAYER_POSITION, // server tells clients to set a players position to this
    JOIN_DATA,           // the data a client needs upon joining
    ADD_PLAYER,

    REQUEST_STATUS // returned to client upon sending a request telling them the status
};

uint64_t GetNextReqestId();

class CSMessage
{
public:
    CSMessageType type;
    CSMessage(){};
    virtual void ToPacket(sf::Packet &packet)
    {
        packet << (uint8_t)type;
    }
    virtual void FromPacket(sf::Packet &packet)
    {
        // uint8_t packetType;
        // packet >> packetType;
        // type = (CSMessageType)packetType;
    }
};



class CSMessageRequest : public CSMessage
{
public:
    uint64_t requestId;
    CSMessageRequest() {
    }
    virtual void ToPacket(sf::Packet &packet)
    {
        CSMessage::ToPacket(packet);
        packet << requestId;
    }
    virtual void FromPacket(sf::Packet &packet)
    {
        CSMessage::FromPacket(packet);
        packet >> requestId;
    }
};

class CSMessageRequestSetPlayerPosition : public CSMessageRequest
{
public:
    sf::Vector2f newPosition;
    CSMessageRequestSetPlayerPosition(){
        type = CSMessageType::REQUEST_SET_PLAYER_POSITION;
    }
    void ToPacket(sf::Packet &packet)
    {
        CSMessage::ToPacket(packet);
        packet << newPosition.x << newPosition.y;
    }
    void FromPacket(sf::Packet &packet)
    {
        CSMessage::FromPacket(packet);
        packet >> newPosition.x >> newPosition.y;
    }
};

class CSMessageSetPlayerPosition : public CSMessage
{
public:
    uint64_t playerId;
    CSMessageSetPlayerPosition(){
        type = CSMessageType::SET_PLAYER_POSITION;
    };
    sf::Vector2f newPosition;
    void ToPacket(sf::Packet &packet)
    {
        CSMessage::ToPacket(packet);
        packet << playerId << newPosition.x << newPosition.y;
    }
    void FromPacket(sf::Packet &packet)
    {
        CSMessage::FromPacket(packet);
        packet >> playerId >> newPosition.x >> newPosition.y;
    }
};

class CSMessageRequestStatus : public CSMessage
{
public:
    uint64_t requestId;
    bool accepted;
    CSMessageRequestStatus(){
        type = CSMessageType::REQUEST_STATUS;
    };
    void FromPacket(sf::Packet &packet)
    {
        CSMessage::ToPacket(packet);
        packet << requestId << accepted;
    }
    void ToPacket(sf::Packet &packet)
    {
        CSMessage::FromPacket(packet);
        packet >> requestId >> accepted;
    }
};

class CSMessageAddPlayer : public CSMessage
{
    public:
    Player player;
    void ToPacket(sf::Packet& packet)
    {
        packet << player.id << player.color.r << player.color.g << player.color.b;
        packet << player.position.x << player.position.y;
    }
    void FromPacket(sf::Packet& packet)
    {
        packet >> player.id >> player.color.r >> player.color.g >> player.color.b;
        packet >> player.position.x >> player.position.y;
    }
};

CSMessage *CSMessageFromType(CSMessageType type);