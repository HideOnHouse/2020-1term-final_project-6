//
// Created by FURO on 2020-05-14.
//

#ifndef INC_2020_1TERM_FINAL_PROJECT_6_WALL_H
#define INC_2020_1TERM_FINAL_PROJECT_6_WALL_H

/*
 * status
 * 0 = Void
 * 1 = Wall
 * 2 = Immune Wall
 * 3 = Snake Head
 * 4 = Snake Tail
 * 5 = Growth Item
 * 6 = Poison Item
 * 7 = Gate
 */
class Wall {
private:
    char status;

public:
    Wall();

    Wall(char Status);

    void setStatus();


};


#endif //INC_2020_1TERM_FINAL_PROJECT_6_WALL_H
