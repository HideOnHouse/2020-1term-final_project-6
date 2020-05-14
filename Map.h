//
// Created by hideoncode on 5/14/20.
//
#ifndef INC_2020_1TERM_FINAL_PROJECT_6_MAP_H
#define INC_2020_1TERM_FINAL_PROJECT_6_MAP_H

#include "Wall.h"

class Map {
private:
    Wall mapArray[21][21]{};
public:
    Map();

    Map(unsigned int size);

    void refreshGate();

    void createItem();

    void deleteItem();

};


#endif //INC_2020_1TERM_FINAL_PROJECT_6_MAP_H
