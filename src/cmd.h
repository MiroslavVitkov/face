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


struct NoOp : Base
{
    void execute() override {}
};


struct Test : Base
{
    enum class Case
    {
        _cam_to_vid,
        _vid_to_vid,
        _dir_to_vid,
        _vid_to_dir,
    };

    Test( Case
        , unsigned frames
        , const std::string & source_path
        , const std::string & dest_path );

    void execute() override;

    const Case _case;
    const unsigned _frames;
    const std::string _source_path;
    const std::string _dest_path;
};


struct CamDetectShow : Base
{
    void execute() override;
};


}  // namespace cmd


#endif  // defined(CMD_H_)