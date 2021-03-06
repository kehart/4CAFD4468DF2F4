
#include <string>
#include <iostream>
#include "point.h"
#include "game.h"
#include "window.h"
using namespace std;

Point::Point(int x, int y, string type, int no, Xwindow *w) 
    : x{x}, y{y}, type{type}, playerNum{no}, w{w} {}


// setter
void Point::setType(string type, bool isBlind) {
    this->type = type;
    const int MIN_ROW=6;
    const int MAX_ROW=15;
    const int MIN_COL=2;
    const int MAX_COL=8;

    bool within=MIN_ROW<=y && y<=MAX_ROW && MIN_COL<=x && x<=MAX_COL;// checks if
    // the value is withint the bounds of the blind effect

    int colour = 0;
    if (type == " ") {
        colour = 0;
    }
    if (type == "I") {
        colour = 2;
    }
    if (type == "J") {
        colour = 3;
    }
    if (type == "L") {
        colour = 4;
    }
    if (type == "O") {
        colour = 5;
    }
    if (type == "S") {
        colour = 6;
    }
    if (type == "T") {
        colour = 7;
    }
    if (type == "X") {
        colour = 1;
    }
    if (type == "Z") {
        colour = 9;
    }
    if (isBlind && within) return;
    
    if (this->w != nullptr) {
        int unit = 18;
        int realX = (x + (playerNum - 1) * 18) * unit;
        int realY = 5 + (y + 3) * unit;
        int realW = unit;
        int realH = unit;
        this->w->fillRectangle(realX, realY, realW, realH, colour);
    }
}


void Point::setID(int n){
    id = n;
}


// getter
int Point::getX() {
    return this->x;
}


int Point::getY() {
    return this->y;
}


string Point::getType() {
    return this->type;
}


int Point::getID(){
    return id;
}


void Point::resetId() {
 id = -1;
}


void Point::reset(bool isBlind) {
    this->type = " ";
    this->setType(" ", isBlind);
}
