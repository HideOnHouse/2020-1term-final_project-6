//
// Created by hideoncode on 6/9/20.
//

#include "stageClass.h"
#include <iostream>

stageClass::stageClass(int whichStage) {
    w = '1';
    i = '2';
    v = ' ';
    switch (whichStage) {
        case 1:
            stageWidth = 25;
            stageHeight = 25;
            itemTick = 30;
            endScore = 10;
            missionGrowth = 7;
            missionPoison = 5;
            missionGate = 3;
            break;
        case 2:
            stageWidth = 30;
            stageHeight = 30;
            itemTick = 30;
            endScore = 10;
            missionGrowth = 7;
            missionPoison = 5;
            missionGate = 3;
            break;
        case 3:
            stageWidth = 30;
            stageHeight = 30;
            itemTick = 30;
            endScore = 10;
            missionGrowth = 7;
            missionPoison = 5;
            missionGate = 3;
            break;
        case 4:
            stageWidth = 35;
            stageHeight = 35;
            itemTick = 30;
            endScore = 10;
            missionGrowth = 7;
            missionPoison = 5;
            missionGate = 3;
            break;
        default:
            std::cout << "No such Stage" << std::endl;
    }
}