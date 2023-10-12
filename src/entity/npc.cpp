#include <entity/npc.hpp>

NPC::NPC(Entity_Data e, sf::Texture& texture)
    : Entity(e, texture)
{}

Conversation NPC::getConversation()
{
    return conversation;
}
