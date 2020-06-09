//
// Created by hideoncode on 6/9/20.
//

#ifndef INC_2020_1TERM_FINAL_PROJECT_6_STAGE_H
#define INC_2020_1TERM_FINAL_PROJECT_6_STAGE_H


class stage {
public:
    int points, tick, stageWidth, stageHeight, itemTick;
    int endScore, missionGrowth, missionPoison, missionGate;
    char wallChar, immuneWallChar, voidChar;

    stage(int whichStage);

};


#endif //INC_2020_1TERM_FINAL_PROJECT_6_STAGE_H
