#ifndef __ICOMMAND_HPP__
#define __ICOMMAND_HPP__

enum class ExecutionPolicy {
    KeepAfterExecution,
    DeleteAfterExecution
};

class ICommand{
private:
    ExecutionPolicy m_Policy;
public:
    ICommand(ExecutionPolicy policy) : m_Policy(policy){}
    virtual ~ICommand(){}

    virtual void Execute() = 0;

    ExecutionPolicy GetExecutionPolicy() const {return m_Policy;}
};

#endif //!__ICOMMAND_HPP__
