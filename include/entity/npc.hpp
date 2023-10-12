#pragma once

#include "entity.hpp"

#include "conversation.hpp"

class NPC : public Entity {
public:
    NPC(Entity_Data e, sf::Texture& texture);

    Conversation getConversation();

private:
    Conversation conversation;
};
