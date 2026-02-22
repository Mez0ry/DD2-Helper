#ifndef __HOTKEY__SEQUENCE_HPP__
#define __HOTKEY__SEQUENCE_HPP__
#include <vector>
#include <string>
#include <cstdint>
#include <windows.h>

enum class VKeys : unsigned long long;

class HotKeySequence{
public:
    HotKeySequence() = default;
    HotKeySequence(const std::string& key_sequence, const char delimiter = '+');
    HotKeySequence(VKeys key, const std::string& key_str = "");

    ~HotKeySequence();

    void AddKey(const std::string& key);
    void AddKey(const VKeys key);

    bool Pressed() const;
    static bool Pressed(const std::string& button);
    static bool Pressed(const VKeys& key);

    size_t GetCurrKeyIndex() const;
    void PressKey(HWND window_descriptor, uint16_t hold_time_ms = 0, uint16_t button_released_time_ms = 0);
    void PressKeys(HWND window_descriptor, uint16_t hold_time_ms = 0U, uint16_t button_released_time_ms = 0U);

    auto begin() const {return m_HotKeySequence.begin();}
    auto end() const {return m_HotKeySequence.end();}

    const auto cbegin() const {return m_HotKeySequence.cbegin();}
    const auto cend() const {return m_HotKeySequence.cend();}

    std::string GetSequenceAsStr(std::string delimiter = "+") const;

    bool IsSingleKey() const;
private:
    static VKeys GetKeyFromStr(const std::string& key);

private:
    std::vector<VKeys> m_HotKeySequence;
    std::vector<std::string> m_HotkeyStrList;
    size_t m_CurrKeyIndex{0ul};
};
#endif // __HOTKEY__SEQUENCE_HPP__
