#ifndef __ICOMMAND_HPP__
#define __ICOMMAND_HPP__
#include <functional>
#include <memory>

class ICommand{
public:
    virtual ~ICommand() {}

    virtual void Execute() = 0;
};

template <typename TCommand>
class IConditionalCommand final : public ICommand {
private:
    std::function<bool()> m_Pred;
    TCommand m_Command;
    
public:
    IConditionalCommand(const TCommand& command, std::function<bool()> pred) : m_Command(command), m_Pred(pred) {}
    ~IConditionalCommand() = default;
    
    void Execute() override {
        if (m_Pred()) {
            m_Command.Execute();
        }
    }
};

template<typename CommandType, typename TPredicate, typename... Args>
inline auto MakeConditionalCommand(TPredicate condition, Args&&... args) {
    auto command_instance = CommandType(std::forward<Args>(args)...);
    return std::make_shared<IConditionalCommand<CommandType>>(command_instance, condition);
}

template<typename CommandType, typename TPredicate>
inline auto MakeConditionalCommand(TPredicate condition) {
    auto command_instance = CommandType();
    return std::make_shared<IConditionalCommand<CommandType>>(
        command_instance,
        condition
    );
}

#endif //! __ICOMMAND_HPP__