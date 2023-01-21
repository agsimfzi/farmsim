#include <util/wrap_text.hpp>

bool charInBounds(sf::Text& text, unsigned int i, int width){
    return (text.findCharacterPos(i).x + text.getCharacterSize()*1.5 - text.findCharacterPos(0).x < width);
}

void wrapText(sf::Text& text, int width){
    std::string toWrap = text.getString();
    std::string wrapped;

    unsigned int epos = 1;

    while(epos < toWrap.length()){
        if(!charInBounds(text, epos, width)){
            bool space = true;
            unsigned int tpos = epos;
            while(toWrap.at(--epos) != ' '){
                if(epos == 0 || toWrap.at(epos) == '\n'){
                    space = false;
                    break;
                }
            }
            if(space){
                wrapped += toWrap.substr(0, epos) + "\n";
                toWrap.erase(epos, 1);
            }
            else{
                epos = tpos;
                while(!charInBounds(text, epos, width)){
                    epos--;
                }
                toWrap.insert(--epos, "-");
                wrapped += toWrap.substr(0, ++epos) + "\n";
            }
            toWrap = toWrap.substr(epos);
            epos = 0;
            continue;
        }
        epos++;
    }
    wrapped += toWrap;

    text.setString(wrapped);
}
