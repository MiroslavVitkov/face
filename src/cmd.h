#ifndef CMD_H_
#define CMD_H_


#include <string>


namespace cmd
{


struct Base
{
    virtual void execute() = 0;
    virtual ~Base() = default;
};


struct PrintHelp : Base
{
    void execute() override;
};


}  // namespace cmd


#endif  // defined(CMD_H_)
