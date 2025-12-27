#ifndef __KEY_SEQUENCE_HPP__
#define __KEY_SEQUENCE_HPP__
#include <vector>
#include <string>
#include <cstdint>
#include <windows.h>

enum class VKeys : unsigned long long;

class KeySequence{
public:
    KeySequence() = default;
    KeySequence(const std::string& key_sequence, const char delimiter = ' ');
    KeySequence(VKeys key);

    ~KeySequence();

    void AddKey(const std::string& key);
    void AddKey(const VKeys key);

    bool IsKeysPressed() const;
    size_t GetCurrKeyIndex() const;
    void PressKey(HWND window_descriptor, uint16_t hold_time_ms = 0, uint16_t button_released_time_ms = 0);
    void PressKeys(HWND window_descriptor, uint16_t hold_time_ms = 0U, uint16_t button_released_time_ms = 0U);

    auto begin() {return m_KeySequence.begin();}
    auto end() {return m_KeySequence.end();}
private:
    std::vector<VKeys> m_KeySequence;
    size_t m_CurrKeyIndex{0ul};
};

#endif //!__KEY_SEQUENCE_HPP__