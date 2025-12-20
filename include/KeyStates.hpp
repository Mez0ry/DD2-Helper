#ifndef __KEY_STATES_HPP__
#define __KEY_STATES_HPP__

enum class KeyState : unsigned long long
{
    None        = 0x0000,
    Pressed     = 0x8000,
    Toggled     = 0x0001,
};

#endif //! __KEY_STATES_HPP__