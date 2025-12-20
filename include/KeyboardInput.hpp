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

    static bool HasButtonKey(const std::string &vkey_str);
    static VKeys VKeyFromStr(const std::string& vkey_str);

    static bool IsKeyPressed(VKeys vkey);

    static bool HoldKey(HWND window_descriptor, VKeys key, uint16_t hold_time_ms);
    static bool ReleaseKey(HWND window_descriptor, VKeys key, uint16_t key_released_time_ms);

    static bool PressKey(HWND window_descriptor, VKeys key, uint16_t hold_time_ms, uint16_t key_released_time_ms);
    
    /**
     * @return true if each key in sequence pressed otherwise false
    */
    static bool PressKeySequense(HWND window_descriptor, const std::string& key_sequence, uint16_t hold_time_ms, uint16_t key_released_time_ms, const char delimiter = ' ');

private:
    static const std::unordered_map<std::string, VKeys> m_VkeysStrMap;
};
#endif //!__KEYBOARD_INPUT_HPP__