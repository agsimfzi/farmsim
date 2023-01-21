#include <audio/sound_interface.hpp>

//////////////////////////////////////////////////////////////

void Sound_Interface::update()
{
    busReader.read(player);
    player.update();
}
