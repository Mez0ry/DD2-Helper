#ifndef __MOUSE_INPUT_HPP__
#define __MOUSE_INPUT_HPP__
#include <windows.h>

#include <unordered_map>
#include <string>
#include <cstdint>

#include "VKeys.hpp"

class MouseInput{
public:
    MouseInput() = delete;

    static bool HasButton(const VKeys key);
    static bool HasButton(const std::string& vbutton_str);
    static VKeys VButtonKeyFromStr(const std::string& vbutton_str);

    static bool IsButtonPressed(VKeys vkey);

    static bool HoldButton(HWND window_descriptor, VKeys key, uint16_t hold_time_ms = 0);
    static bool ReleaseButton(HWND window_descriptor, VKeys key, uint16_t button_released_time_ms = 0);

    static bool PressButton(HWND window_descriptor, VKeys key, uint16_t hold_time_ms = 0, uint16_t button_released_time_ms = 0);
    static bool PressSequence(HWND window_descriptor, const std::string& button_sequence, uint16_t hold_time_ms, uint16_t button_released_time_ms, const char delimiter = '+');

private:
    static const std::unordered_map<std::string, VKeys> m_VButtonStrMap;
};
#endif //!__MOUSE_INPUT_HPP__
