#include "CSMessage.hpp"


uint64_t GetNextReqestId()
{
    static uint64_t nextId = 0;
    return nextId++;
}

CSMessage *CSMessageFromType(CSMessageType type)
{
    CSMessage *msg;
    if (type == CSMessageType::REQUEST_SET_PLAYER_POSITION)
    {
        msg = new CSMessageRequestSetPlayerPosition();
    }else if (type == CSMessageType::SET_PLAYER_POSITION)
    {
        msg = new CSMessageSetPlayerPosition();
    }else if (type == CSMessageType::REQUEST_STATUS)
    {
        msg = new CSMessageRequestStatus();
    }else if (type == CSMessageType::ADD_PLAYER)
    {
        msg = new CSMessageAddPlayer();
    }else
    {
        msg = new CSMessage();
    }
    msg->type = type;
    return msg;
}