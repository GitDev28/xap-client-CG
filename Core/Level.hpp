#pragma once
#include "Offsets.hpp"
#include "../Utils/Memory.hpp"

struct Level {
    std::string Name;
    bool IsPlayable;
    bool IsFiringRange;

    long getBasePointer()
    {
        long basePointer = OFF_REGION + OFF_LEVEL;
        return basePointer;
    }
    std::string getName()
    {
        long basePointer = getBasePointer();
        std::string result = Memory::ReadString(basePointer);
        return result;
    }
    bool isPlayable()
    {
        if (getName().empty())
            return false;
        if (getName().compare("mp_lobby") == 0)
            return false;
        return true;
    }
    bool isTrainingArea()
    {
        if (getName().compare("mp_rr_canyonlands_staging_mu1") == 0)
            return true;
        return false;
    }

    void readFromMemory() {
        Name = Memory::ReadString(OFF_REGION + OFF_LEVEL, 1024, "Level name");
        IsPlayable = isPlayable();
        IsFiringRange = isTrainingArea();
    }
};