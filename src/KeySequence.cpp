#include "KeySequence.hpp"
#include "VKeys.hpp"
#include "KeyboardInput.hpp"
#include "MouseInput.hpp"
#include <algorithm>
#include <sstream>

KeySequence::KeySequence(const std::string &key_sequence, const char delimiter)
{
    std::stringstream ss(key_sequence);
    std::string item;

    while (std::getline(ss, item, delimiter))
    {
        size_t start = item.find_first_not_of(" \t");
        size_t end = item.find_last_not_of(" \t");

        if (start != std::string::npos && end != std::string::npos){
            item = item.substr(start, end - start + 1);
        }

        AddKey(item);
    }
}

KeySequence::KeySequence(VKeys key)
{
    this->AddKey(key);
}

KeySequence::~KeySequence() {}

void KeySequence::AddKey(const std::string &key)
{
    if(MouseInput::HasButton(key)){
        m_KeySequence.push_back(MouseInput::VButtonKeyFromStr(key));
    }else if(KeyboardInput::HasKey(key)){
        m_KeySequence.push_back(KeyboardInput::VKeyFromStr(key));
    }
}

void KeySequence::AddKey(const VKeys key) {
    m_KeySequence.push_back(key);
}

bool KeySequence::IsKeysPressed() const
{
    std::vector<bool> results_vec;

    for(const auto& key : m_KeySequence){
        results_vec.push_back(KeyboardInput::IsKeyPressed(key) || MouseInput::IsButtonPressed(key));
    }

    return !(std::any_of(results_vec.begin(), results_vec.end(), [](int value){
        return (value == 0);
    }));
}

size_t KeySequence::GetCurrKeyIndex() const
{
    return m_CurrKeyIndex;
}

void KeySequence::PressKey(HWND window_descriptor, uint16_t hold_time_ms, uint16_t button_released_time_ms) {
    if(m_KeySequence.empty()){
        return;
    }

    auto key = m_KeySequence[m_CurrKeyIndex];

    if(MouseInput::HasButton(key)){
        MouseInput::PressButton(window_descriptor, key, hold_time_ms);
    }else if(KeyboardInput::HasKey(key)){
        KeyboardInput::PressKey(window_descriptor, key, button_released_time_ms);
    }

    ++m_CurrKeyIndex;

    if(m_CurrKeyIndex >= m_KeySequence.size()){
        m_CurrKeyIndex = 0;
    }
}

void KeySequence::PressKeys(HWND window_descriptor, uint16_t hold_time_ms, uint16_t button_released_time_ms) {
    for(const auto& key : m_KeySequence){
        if(MouseInput::HasButton(key)){
            MouseInput::PressButton(window_descriptor, key, hold_time_ms, button_released_time_ms);
        }else if(KeyboardInput::HasKey(key)){
            KeyboardInput::PressKey(window_descriptor, key, hold_time_ms, button_released_time_ms);
        }
    }
}
