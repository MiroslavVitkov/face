#ifndef CMD_H_
#define CMD_H_


#include "algo.h"

#include <memory>
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


// camera -> detect faces -> play video
struct CamDetectShow : Base
{
    void execute() override;
};


// Tell the model who the person on the carera is.
struct CamTrain : Base
{
    CamTrain( const std::string & label
            , const std::string & fname_model = "model" );  // created if missing
    ~CamTrain() override;
    void execute() override;

private:
    algo::TrainerLBP _model;
};


struct CamRecognise : Base
{
    using Label = int;

    CamRecognise( const std::string & fname_model = "model" );
    void execute() override;
    ~CamRecognise() override;

private:
    struct Impl;
    std::unique_ptr<Impl> _impl;
};


}  // namespace cmd


#endif  // defined(CMD_H_)
