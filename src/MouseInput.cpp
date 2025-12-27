#include "MouseInput.hpp"
#include "KeyStates.hpp"

#include <stdexcept>
#include <vector>
#include <sstream>
#include <algorithm>

const std::unordered_map<std::string, VKeys> MouseInput::m_VButtonStrMap = {
    {"LBUTTON", VKeys::LBUTTON},
    {"RBUTTON", VKeys::RBUTTON},
    {"CANCEL", VKeys::CANCEL},
    {"MBUTTON", VKeys::MBUTTON},
    {"X_BUTTON1", VKeys::X_BUTTON1},
    {"X_BUTTON2", VKeys::X_BUTTON2},
};

bool MouseInput::HasButton(const VKeys key)
{
    auto it = std::find_if(m_VButtonStrMap.begin(), m_VButtonStrMap.end(), [&](const decltype(m_VButtonStrMap)::value_type &v) { return v.second == key; });
    return (it != m_VButtonStrMap.end());
}

bool MouseInput::HasButton(const std::string &vbutton_str)
{
    return m_VButtonStrMap.find(vbutton_str) != m_VButtonStrMap.end();
}

VKeys MouseInput::VButtonKeyFromStr(const std::string &vbutton_str) {
  auto it = m_VButtonStrMap.find(vbutton_str);

  if (it == m_VButtonStrMap.end()) {
    throw std::invalid_argument("Specified virtual key doesnt exists");
  }

  return it->second;
}

bool MouseInput::IsButtonPressed(VKeys vkey)
{
    return (GetAsyncKeyState(static_cast<int>(vkey)) & static_cast<uint64_t>(KeyState::Pressed)) != 0;
}

bool MouseInput::HoldButton(HWND window_descriptor, VKeys key, uint16_t hold_time_ms)
{
    PostMessageW(window_descriptor, WM_KEYDOWN, static_cast<WPARAM>(key), 0);
    Sleep(hold_time_ms); 
    return true;
}

bool MouseInput::ReleaseButton(HWND window_descriptor, VKeys key, uint16_t button_released_time_ms)
{
    PostMessageW(window_descriptor, WM_KEYUP, static_cast<WPARAM>(key), 0);
    Sleep(button_released_time_ms); 
    return true;
}

bool MouseInput::PressButton(HWND window_descriptor, VKeys key, uint16_t hold_time_ms, uint16_t button_released_time_ms)
{
    return HoldButton(window_descriptor, key, hold_time_ms) && ReleaseButton(window_descriptor, key, button_released_time_ms);
}

bool MouseInput::PressSequence(HWND window_descriptor, const std::string &button_sequence, uint16_t hold_time_ms, uint16_t button_released_time_ms, const char delimiter)
{
    std::string seq_cpy = button_sequence;
    std::vector<std::string> buttons;

    std::stringstream ss(seq_cpy);
    std::string item;

    while (std::getline(ss, item, delimiter))
    {
        size_t start = item.find_first_not_of(" \t");
        size_t end = item.find_last_not_of(" \t");

        if (start != std::string::npos && end != std::string::npos){
            item = item.substr(start, end - start + 1);
        }

        buttons.push_back(item);
    }

    bool success = false;

    for(auto& button : buttons){
        PressButton(window_descriptor, MouseInput::VButtonKeyFromStr(button), hold_time_ms, button_released_time_ms);
    }

    return success;
}
