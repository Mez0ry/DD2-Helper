#include "HotKeySequence.hpp"
#include "VKeys.hpp"
#include "KeyboardInput.hpp"
#include "MouseInput.hpp"
#include <algorithm>
#include <sstream>

HotKeySequence::HotKeySequence(const std::string &key_sequence, const char delimiter)
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

        m_HotkeyStrList.push_back(item);
        AddKey(item);
    }
}

HotKeySequence::HotKeySequence(VKeys key, const std::string& key_str)
{
    this->m_HotkeyStrList.push_back(key_str);
    this->AddKey(key);
}

HotKeySequence::~HotKeySequence() {}

void HotKeySequence::AddKey(const std::string &key)
{
    if(MouseInput::HasButton(key)){
        m_HotKeySequence.push_back(MouseInput::VButtonKeyFromStr(key));
    }else if(KeyboardInput::HasKey(key)){
        m_HotKeySequence.push_back(KeyboardInput::VKeyFromStr(key));
    }
}

void HotKeySequence::AddKey(const VKeys key) {
    m_HotKeySequence.push_back(key);
}

bool HotKeySequence::Pressed() const
{
    std::vector<bool> results_vec;

    for(const auto& key : m_HotKeySequence){
        results_vec.push_back(Pressed(key));
    }

    return !(std::any_of(results_vec.begin(), results_vec.end(), [](int value){
        return (value == 0);
    }));
}

bool HotKeySequence::Pressed(const std::string &button) {
    const auto vkey = GetKeyFromStr(button);
    if(vkey == VKeys::UNKNOWN){
        return false;
    }
    return (KeyboardInput::IsKeyPressed(vkey) || MouseInput::IsButtonPressed(vkey));
}

bool HotKeySequence::Pressed(const VKeys &key) {
    if(key == VKeys::UNKNOWN){
        return false;
    }
    return (KeyboardInput::IsKeyPressed(key) || MouseInput::IsButtonPressed(key));
}

size_t HotKeySequence::GetCurrKeyIndex() const
{
    return m_CurrKeyIndex;
}

void HotKeySequence::PressKey(HWND window_descriptor, uint16_t hold_time_ms, uint16_t button_released_time_ms) {
    if(m_HotKeySequence.empty()){
        return;
    }

    auto key = m_HotKeySequence[m_CurrKeyIndex];

    if(MouseInput::HasButton(key)){
        MouseInput::PressButton(window_descriptor, key, hold_time_ms);
    }else if(KeyboardInput::HasKey(key)){
        KeyboardInput::PressKey(window_descriptor, key, button_released_time_ms);
    }

    ++m_CurrKeyIndex;

    if(m_CurrKeyIndex >= m_HotKeySequence.size()){
        m_CurrKeyIndex = 0;
    }
}

void HotKeySequence::PressKeys(HWND window_descriptor, uint16_t hold_time_ms, uint16_t button_released_time_ms) {
    for(const auto& key : m_HotKeySequence){
        if(MouseInput::HasButton(key)){
            MouseInput::PressButton(window_descriptor, key, hold_time_ms, button_released_time_ms);
        }else if(KeyboardInput::HasKey(key)){
            KeyboardInput::PressKey(window_descriptor, key, hold_time_ms, button_released_time_ms);
        }
    }
}

std::string HotKeySequence::GetSequenceAsStr(std::string delimiter) const{
    if(m_HotkeyStrList.empty()){
        return "";
    }

    std::string sequence;
    if(m_HotkeyStrList.size() == 1 ){
        return m_HotkeyStrList[0];
    }

    for(auto index = 0; index < m_HotkeyStrList.size(); ++index){
        const std::string delimiter_symbol = (index + 1 < m_HotkeyStrList.size()) ? delimiter : "";

        sequence += m_HotkeyStrList[index] + delimiter_symbol;
    }

    return sequence;
}

bool HotKeySequence::IsSingleKey() const
{
    return (m_HotKeySequence.size() == 1 && m_HotkeyStrList.size() == 1);
}

VKeys HotKeySequence::GetKeyFromStr(const std::string &key) {
    if(MouseInput::HasButton(key)){
        return MouseInput::VButtonKeyFromStr(key);
    }else if(KeyboardInput::HasKey(key)){
        return KeyboardInput::VKeyFromStr(key);
    }

    return VKeys::UNKNOWN;
}
