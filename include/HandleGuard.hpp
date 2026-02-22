#ifndef __HANDLE_GUARD_HPP__
#define __HANDLE_GUARD_HPP__
#include <Windows.h>

class HandleGuard
{
private:
    HANDLE m_Handle;
public:
    HandleGuard(HANDLE handle) : m_Handle(handle)
    {

    }

    ~HandleGuard()
    {
        CloseHandle(m_Handle);
    }

    HANDLE GetHandle() const
    {
        return m_Handle;
    }

    HandleGuard operator=(HANDLE handle)
    {
        return HandleGuard(handle);
    }

    operator void *()
    {
        return m_Handle;
    }
};
#endif //!__HANDLE_GUARD_HPP__
