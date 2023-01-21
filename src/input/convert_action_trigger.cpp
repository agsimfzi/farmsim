#include <input/convert_action_trigger.hpp>

//////////////////////////////////////////////////////////////

std::map<sf::Keyboard::Key, std::string> Convert_Action_Trigger::key_to_string;
std::map<std::string, sf::Keyboard::Key> Convert_Action_Trigger::string_to_key;

std::map<sf::Mouse::Button, std::string> Convert_Action_Trigger::button_to_string;
std::map<std::string, sf::Mouse::Button> Convert_Action_Trigger::string_to_button;

Convert_Action_Trigger::Convert_Action_Trigger()
{
    if (key_to_string.empty()) {
        init();
    }
}

std::string Convert_Action_Trigger::toString(const Action_Trigger& trigger)
{
    std::string str { "huh?" };

    if (std::holds_alternative<sf::Keyboard::Key>(trigger)) {
        str = key_to_string[std::get<sf::Keyboard::Key>(trigger)];
    }
    else if (std::holds_alternative<sf::Mouse::Button>(trigger)) {
        str = button_to_string[std::get<sf::Mouse::Button>(trigger)];
    }
    else if (std::holds_alternative<std::string>(trigger)) {
        str = std::get<std::string>(trigger);
    }

    return str;
}

Action_Trigger Convert_Action_Trigger::toKey(const std::string& str)
{
    Action_Trigger key;

    if (string_to_key.contains(str)) {
        key.emplace<sf::Keyboard::Key>(string_to_key[str]);
    }
    else if (string_to_button.contains(str)) {
        key.emplace<sf::Mouse::Button>(string_to_button[str]);
    }
    else {
        key.emplace<std::string>("failed to parse");
    }

    return key;
}

void Convert_Action_Trigger::init()
{
    button_to_string[sf::Mouse::Left] = "LMouse";
    button_to_string[sf::Mouse::Right] = "RMouse";
    string_to_button["LMouse"] = sf::Mouse::Left;
    string_to_button["RMouse"] = sf::Mouse::Right;

#define KEY_INSERT(x)                                           \
    key_to_string.insert({ sf::Keyboard::x, std::string(#x) }); \
    string_to_key.insert({ std::string(#x), sf::Keyboard::x });

    KEY_INSERT(Unknown);
    KEY_INSERT(A);
    KEY_INSERT(B);
    KEY_INSERT(C);
    KEY_INSERT(D);
    KEY_INSERT(E);
    KEY_INSERT(F);
    KEY_INSERT(G);
    KEY_INSERT(H);
    KEY_INSERT(I);
    KEY_INSERT(J);
    KEY_INSERT(K);
    KEY_INSERT(L);
    KEY_INSERT(M);
    KEY_INSERT(N);
    KEY_INSERT(O);
    KEY_INSERT(P);
    KEY_INSERT(Q);
    KEY_INSERT(R);
    KEY_INSERT(S);
    KEY_INSERT(T);
    KEY_INSERT(U);
    KEY_INSERT(V);
    KEY_INSERT(W);
    KEY_INSERT(X);
    KEY_INSERT(Y);
    KEY_INSERT(Z);
    KEY_INSERT(Num0);
    KEY_INSERT(Num1);
    KEY_INSERT(Num2);
    KEY_INSERT(Num3);
    KEY_INSERT(Num4);
    KEY_INSERT(Num5);
    KEY_INSERT(Num6);
    KEY_INSERT(Num7);
    KEY_INSERT(Num8);
    KEY_INSERT(Num9);
    KEY_INSERT(Escape);
    KEY_INSERT(LControl);
    KEY_INSERT(LShift);
    KEY_INSERT(LAlt);
    KEY_INSERT(LSystem);
    KEY_INSERT(RControl);
    KEY_INSERT(RShift);
    KEY_INSERT(RAlt);
    KEY_INSERT(RSystem);
    KEY_INSERT(Menu);
    KEY_INSERT(LBracket);
    KEY_INSERT(RBracket);
    KEY_INSERT(SemiColon);
    KEY_INSERT(Comma);
    KEY_INSERT(Period);
    KEY_INSERT(Quote);
    KEY_INSERT(Slash);
    KEY_INSERT(BackSlash);
    KEY_INSERT(Tilde);
    KEY_INSERT(Equal);
    KEY_INSERT(Dash);
    KEY_INSERT(Space);
    KEY_INSERT(Return);
    KEY_INSERT(BackSpace);
    KEY_INSERT(Tab);
    KEY_INSERT(PageUp);
    KEY_INSERT(PageDown);
    KEY_INSERT(End);
    KEY_INSERT(Home);
    KEY_INSERT(Insert);
    KEY_INSERT(Delete);
    KEY_INSERT(Add);
    KEY_INSERT(Subtract);
    KEY_INSERT(Multiply);
    KEY_INSERT(Divide);
    KEY_INSERT(Left);
    KEY_INSERT(Right);
    KEY_INSERT(Up);
    KEY_INSERT(Down);
    KEY_INSERT(Numpad0);
    KEY_INSERT(Numpad1);
    KEY_INSERT(Numpad2);
    KEY_INSERT(Numpad3);
    KEY_INSERT(Numpad4);
    KEY_INSERT(Numpad5);
    KEY_INSERT(Numpad6);
    KEY_INSERT(Numpad7);
    KEY_INSERT(Numpad8);
    KEY_INSERT(Numpad9);
    KEY_INSERT(F1);
    KEY_INSERT(F2);
    KEY_INSERT(F3);
    KEY_INSERT(F4);
    KEY_INSERT(F5);
    KEY_INSERT(F6);
    KEY_INSERT(F7);
    KEY_INSERT(F8);
    KEY_INSERT(F9);
    KEY_INSERT(F10);
    KEY_INSERT(F11);
    KEY_INSERT(F12);
    KEY_INSERT(F13);
    KEY_INSERT(F14);
    KEY_INSERT(F15);
    KEY_INSERT(Pause);

    static_assert(sf::Keyboard::KeyCount == 101, "Number of SFML keys has changed");
#undef KEY_INSERT
}
