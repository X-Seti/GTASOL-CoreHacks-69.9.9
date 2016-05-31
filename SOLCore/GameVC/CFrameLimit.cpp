/*
 *  FrameLimit Adjuster
 *
 */
#include "../LimitAdjuster.h"
 
class FrameLimitVC : public SimpleAdjuster
{
public:
    const char* GetLimitName() {"FrameLimit", nullptr;}
    void ChangeLimit(int, const std::string& value) 
    { 
        injector::WriteMemory(0x602D68, std::stoi(value), true); 
        injector::WriteMemory(0x9B48EC, std::stoi(value), true);
    }

} FrameLimitVC;
