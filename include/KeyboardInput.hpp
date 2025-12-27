#ifndef __KEYBOARD_INPUT_HPP__
#define __KEYBOARD_INPUT_HPP__
#include <windows.h>
#include <unordered_map>
#include <string>
#include <cstdint>

#include "VKeys.hpp"

class KeyboardInput{
public:
    KeyboardInput() = delete;

    static bool HasKey(const VKeys key);
    static bool HasKey(const std::string &vkey_str);
    static VKeys VKeyFromStr(const std::string& vkey_str);

    static bool IsKeyPressed(VKeys vkey);

    static bool HoldKey(HWND window_descriptor, VKeys key, uint16_t hold_time_ms = 0);
    static bool ReleaseKey(HWND window_descriptor, VKeys key, uint16_t key_released_time_ms = 0);

    static bool PressKey(HWND window_descriptor, VKeys key, uint16_t hold_time_ms = 0, uint16_t key_released_time_ms = 0);
private:
    static const std::unordered_map<std::string, VKeys> m_VkeysStrMap;
};

#endif //!__KEYBOARD_INPUT_HPP__