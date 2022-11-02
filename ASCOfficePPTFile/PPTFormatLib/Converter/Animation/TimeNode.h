#pragma once

#include <vector>
#include <memory>

namespace PPT {
namespace Intermediate {
class Animation;
struct TimeNode
{
    long cTnId = -1;
    std::shared_ptr<Animation> anim;
    std::vector<std::shared_ptr<TimeNode>> childNodes;

protected:
    TimeNode();
};

struct ParNode : TimeNode
{

};

struct SeqNode : TimeNode
{

};
}
}
