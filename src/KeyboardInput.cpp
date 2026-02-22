#include "KeyboardInput.hpp"

#include <stdexcept>
#include <algorithm>

const std::unordered_map<std::string, VKeys> KeyboardInput::m_VkeysStrMap = {
    {"BACK", VKeys::BACK},
    {"TAB", VKeys::TAB},
    {"CLEAR", VKeys::CLEAR},
    {"RETURN", VKeys::RETURN},
    {"SHIFT", VKeys::SHIFT},
    {"CONTROL", VKeys::CONTROL},
    {"MENU", VKeys::MENU},
    {"PAUSE", VKeys::PAUSE},
    {"CAPITAL", VKeys::CAPITAL},
    {"ESCAPE", VKeys::ESCAPE},
    {"SPACE", VKeys::SPACE},
    {"PRIOR", VKeys::PRIOR},
    {"NEXT", VKeys::NEXT},
    {"END", VKeys::END},
    {"HOME", VKeys::HOME},
    {"LEFT", VKeys::LEFT},
    {"UP", VKeys::UP},
    {"RIGHT", VKeys::RIGHT},
    {"DOWN", VKeys::DOWN},
    {"INSERT", VKeys::INSERT},
    {"DELETE", VKeys::KDELETE},
    {"K0", VKeys::K0},
    {"K1", VKeys::K1},
    {"K2", VKeys::K2},
    {"K3", VKeys::K3},
    {"K4", VKeys::K4},
    {"K5", VKeys::K5},
    {"K6", VKeys::K6},
    {"K7", VKeys::K7},
    {"K8", VKeys::K8},
    {"K9", VKeys::K9},
    {"A", VKeys::A},
    {"B", VKeys::B},
    {"C", VKeys::C},
    {"D", VKeys::D},
    {"E", VKeys::E},
    {"F", VKeys::F},
    {"G", VKeys::G},
    {"H", VKeys::H},
    {"I", VKeys::I},
    {"J", VKeys::J},
    {"K", VKeys::K},
    {"L", VKeys::L},
    {"M", VKeys::M},
    {"N", VKeys::N},
    {"O", VKeys::O},
    {"P", VKeys::P},
    {"Q", VKeys::Q},
    {"R", VKeys::R},
    {"S", VKeys::S},
    {"T", VKeys::T},
    {"U", VKeys::U},
    {"V", VKeys::V},
    {"W", VKeys::W},
    {"X", VKeys::X},
    {"Y", VKeys::Y},
    {"Z", VKeys::Z},
    {"NUMPAD0", VKeys::NUMPAD0},
    {"NUMPAD1", VKeys::NUMPAD1},
    {"NUMPAD2", VKeys::NUMPAD2},
    {"NUMPAD3", VKeys::NUMPAD3},
    {"NUMPAD4", VKeys::NUMPAD4},
    {"NUMPAD5", VKeys::NUMPAD5},
    {"NUMPAD6", VKeys::NUMPAD6},
    {"NUMPAD7", VKeys::NUMPAD7},
    {"NUMPAD8", VKeys::NUMPAD8},
    {"NUMPAD9", VKeys::NUMPAD9},
    {"F1", VKeys::F1},
    {"F2", VKeys::F2},
    {"F3", VKeys::F3},
    {"F4", VKeys::F4},
    {"F5", VKeys::F5},
    {"F6", VKeys::F6},
    {"F7", VKeys::F7},
    {"F8", VKeys::F8},
    {"F9", VKeys::F9},
    {"F10", VKeys::F10},
    {"F11", VKeys::F11},
    {"F12", VKeys::F12},
    {"F13", VKeys::F13},
    {"F14", VKeys::F14},
    {"F15", VKeys::F15},
    {"F16", VKeys::F16},
    {"F17", VKeys::F17},
    {"F18", VKeys::F18},
    {"F19", VKeys::F19},
    {"F20", VKeys::F20},
    {"F21", VKeys::F21},
    {"F22", VKeys::F22},
    {"F23", VKeys::F23},
    {"F24", VKeys::F24}
};

bool KeyboardInput::HasKey(const VKeys key)
{
    auto it = std::find_if(m_VkeysStrMap.begin(), m_VkeysStrMap.end(), [&](const decltype(m_VkeysStrMap)::value_type &v) { return v.second == key; });
    return (it != m_VkeysStrMap.end());
}

bool KeyboardInput::HasKey(const std::string &vkey_str)
{
    return m_VkeysStrMap.find(vkey_str) != m_VkeysStrMap.end();
}

VKeys KeyboardInput::VKeyFromStr(const std::string &vkey_str)
{
   auto it = m_VkeysStrMap.find(vkey_str);

   if(it == m_VkeysStrMap.end()){
        throw std::invalid_argument("Specified virtual key doesnt exists");
   }

   return it->second;
}

bool KeyboardInput::IsKeyPressed(VKeys vkey)
{
    return (GetAsyncKeyState(static_cast<int>(vkey)) & static_cast<uint64_t>(KeyState::Pressed)) != 0;
}

bool KeyboardInput::HoldKey(HWND window_descriptor, VKeys key, uint16_t hold_time_ms)
{
    if(PostMessageW(window_descriptor, WM_KEYDOWN, static_cast<WPARAM>(key), 0) == NULL){
        return false;
    }
    Sleep(hold_time_ms);
    return true;
}

bool KeyboardInput::ReleaseKey(HWND window_descriptor, VKeys key, uint16_t key_released_time_ms)
{
    if(PostMessageW(window_descriptor, WM_KEYUP, static_cast<WPARAM>(key), 0) == NULL){
        return false;
    }
    Sleep(key_released_time_ms);

    return true;
}

bool KeyboardInput::PressKey(HWND window_descriptor, VKeys key, uint16_t hold_time_ms, uint16_t key_released_time_ms)
{
    return HoldKey(window_descriptor, key, hold_time_ms) && ReleaseKey(window_descriptor, key, key_released_time_ms);
}
