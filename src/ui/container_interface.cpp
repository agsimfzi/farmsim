#include <ui/container_interface.hpp>

Container_Interface::Container_Interface(Player_Inventory& inventory, sf::View& view, Container* container)
    : Inventory_Interface::Inventory_Interface(inventory, view)
    , container{ container }
{}

