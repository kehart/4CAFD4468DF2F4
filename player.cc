#include "player.h"
#include "abstractPlayer.h"
#include "game.h"
#include "blockI.h"
#include <memory>
#include <iostream>
#include "window.h"
using namespace std;

// initialzie the player, draw in the constructor, save window pointer
Player::Player(Game *game, int no, Xwindow *w, int startLvl, std::string scpt):
AbstractPlayer{game, no, w, scpt}{
    //initialize
if(startLvl==1){
            level.reset(new LevelOne);
        }else if(startLvl==2){
            level.reset(new LevelTwo);

        }else if(startLvl==3){
            level.reset(new LevelThree);

        }else if(startLvl ==4){
            level.reset(new LevelFour);

        }else{
            //default to 0
            level.reset(new LevelZero{scpt});
        }
        currentBlock.reset(level->generateBlock());
        currentBlock->initialize(this);
        nextBlock.reset(level->generateBlock());
        if (game->getWindow() != nullptr) {
        game->drawBigString(1, 1, "Level:", this->no);
        game->drawBigString(9, 1, to_string(startLvl), this->no);
        game->drawBigString(1, 2, "Score:", this->no);
        game->drawBigString(9, 2, to_string(currentScore), this->no);
        game->drawBigString(0, 24, " Next:", this->no);
        game->drawBigString(4, 24, "High Score:", this->no);
        game->drawBigString(10, 24, to_string(highestScore), this->no);
            for (auto &p : this->nextBlock->getPositions()) {
                int colour = 0;
                string type = this->nextBlock->getType();
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
                    colour = 0;
                }
                if (type == "Z") {
                    colour = 9;
                }
                game->drawPoint(p.first, p.second + this->rowNum + 5, 1, 1, colour, this->no);
            }
        }
}

int Player::getLevel() {
    return level->getLevel();
}
string Player::getGridRow(int row) {
string s;
    for (int i = 0; i < colNum; i++) {
        s += grid[row][i].getType();
    }
    return s;
}

void Player::setLevel(int level){
    if(this->level != nullptr) this->undrawLevel();
    if (level == 0) {
        this->level.reset(new LevelZero{initScpt});
    } else if (level == 1) {
        this->level.reset(new LevelOne);
    }else if (level == 2){
        this->level.reset(new LevelTwo);
    }else if (level == 3){
        this->level.reset(new LevelThree);
    }else if ( level == 4){
        this->level.reset(new LevelFour);
    }
    this->drawLevel();
}

string Player::getGridPoint(int row, int col) {
 return grid[row][col].getType();
}

void Player::drawGridPoint(int x, int y, int col) {
 game->drawPoint(x, y, 1, 1, col, no);
 }

void Player::undrawGridPoint(int x, int y) {
cout << "player:: undrawGridPoint" << endl;
game->undrawPoint(x, y, 1, 1, no);
 }

shared_ptr<AbstractPlayer> Player::getUnderlyingPlayer() { // there is no underlying player
 return nullptr;
}

void Player::setUnderlyingPlayer(shared_ptr<AbstractPlayer> play) {
 //does nothing
 }

void Player::nullifyUnderlyingPlayer() {
} // there is no underlying player to set to null

int Player::move(string type, int step, bool isBlind) {
cout<<"entering player move"<<endl;
bool succeeded = moveHelper(type, step, isBlind);
cout<<"finished succeed"<<endl;
 int newStep = step;
 // if unable to move step times, move max allowed times strictly less than step and return how
 // many moves were able to be made
 while (!succeeded) { // && (newStep - i >= 0)) {
  newStep -= 1;
  succeeded = moveHelper(type, newStep, isBlind);
  }
  return newStep;
}

    bool Player::moveHelper(std::string type, int step, bool isBlind) {
    if (step == 0) return true; // no movement required
        // interpret command
        int deltaX = 0;
        int deltaY = 0;
        if (type == "down") {
            deltaY = step;
        }
        if (type == "left") {
            deltaX = -step;
        }
        if (type == "right") {
            deltaX = step;
        }
        // check if it's movable
        vector<pair<int, int>> coordinates;
        for (Point *p : currentBlock->getPoints()) {
            pair<int, int> c = make_pair(p->getX() + deltaX, p->getY() + deltaY);
            if (isValid(c)) {
                coordinates.emplace_back(c);
            } else {
                return false;
            }
        }
        // clear block first, then add points
        currentBlock->removeAllPoint(isBlind);
        cout << "add points in move" << endl;
        currentBlock->addPoints(coordinates, this, isBlind);
        return true;
    }

    pair<int, int> getLowerLeft(vector<pair<int, int>> &coordinates) {
     int minX = coordinates[0].first;
     int maxY = coordinates[0].second;
     for (int i = 0; i < (int)coordinates.size(); ++i) {
      if (coordinates[i].first < minX) minX = coordinates[i].first;
      if (coordinates[i].second > maxY) maxY = coordinates[i].second;
      }
     pair<int, int> lowerLeft = make_pair(minX, maxY);
     return lowerLeft;
    }

int Player::rotate(bool counter, int step, bool isBlind) {
    bool succeeded = rotateHelper(counter, step, isBlind);
     int newStep = step;
     // if unable to move step times, move max allowed times strictly less than step and return how
     // many moves were able to be made
 while (!succeeded) { // && (newStep - i >= 0)) {
  newStep -= 1;
  succeeded = rotateHelper(counter, newStep, isBlind);
  }
  return newStep;
}

bool Player::rotateHelper(bool counter, int step, bool isBlind) {
    // if it's a multiple of 4 it ends up in the same place
    if (step % 4 == 0) return true; // end position will be same as start position

    // rotation matrix
    vector<vector<int>> clockWise{{0,-1},{1,0}};
    vector<vector<int>> counterClockWise{{0,1},{-1,0}};
    vector<vector<int>>& current = clockWise;
    if (counter) {
        current = counterClockWise;
    }
    int numRotations = step % 4; // since 4 possible orientations
    vector<pair<int, int>> coordinates; // create cordinates array foro case rot is 0
    for (Point *p : currentBlock->getPoints()) {
     int x = p->getX();
     int y = p->getY();
     pair<int, int> pair = make_pair(x, y);
     coordinates.emplace_back(pair);
     }

   pair<int, int> lowerLeft;
    
    for (int i = 0; i < numRotations; ++i) {
    lowerLeft = getLowerLeft(coordinates);
    vector<pair<int, int>> coorCopy;
    for (pair<int, int> p : coordinates) {
        int x = p.first;
        int y = p.second;
        int newX = x * current[0][0] + y * current[0][1];
        int newY = x * current[1][0] + y * current[1][1];
        pair<int, int> p1 = make_pair(newX, newY);
        coorCopy.emplace_back(p1);
        }

      pair<int, int> rotatedll = getLowerLeft(coorCopy);
      int deltaX = rotatedll.first - lowerLeft.first;
      int deltaY = rotatedll.second - lowerLeft.second;
      coordinates.clear();
      for (pair<int, int> p : coorCopy) {
        pair<int, int> finalPair = make_pair(p.first - deltaX, p.second - deltaY);
      
        if (isValid(finalPair)) {
            coordinates.emplace_back(finalPair);
        } else {
            return false;
        }
       }
     }
    // clear block first, then add points
    currentBlock->removeAllPoint(isBlind);
    cout << "add points in roaotte" << endl;
    currentBlock->addPoints(coordinates, this, isBlind);
    return true;
}


void Player::clearBlind() {
for (int i = 3; i <= 12 + reservedRowNum; ++i) {
  for (int j = 2; j <= 8; ++j) {
        string type = getGridPoint(i, j); 
        setGridType(i,j,type);
  }
 }
}

// add the points of blocks to grid, update the block in drop(), 
void Player::drop(bool shouldClearBlind){
cout<<"entering drop"<<endl;
    while (move("down", 1, shouldClearBlind)) {}
    cout<<"after move down"<<endl;
    currentBlock->setID(AbstractBlock::getCurId());
    cout<<"finished setting id"<<endl;
    inactiveBlocks[AbstractBlock::getCurId()] = std::move(currentBlock);
    cout<<"added to inactive blocks"<<endl;
    AbstractBlock::incrementCurId();
    cout<<"incremented current id"<<endl;
    this->undrawNextBlock();
    cout<<"undraw next block"<<endl;
    currentBlock = std::move(nextBlock);
    cout<<"moved next block to current block"<<endl;
    currentBlock->initialize(this);
    cout<<"initialized current blck's points"<<endl;
    unique_ptr<AbstractBlock>tmp{level->generateBlock()};
    cout<<"created tmp from generate block"<<endl;
    nextBlock = std::move(tmp);
    cout<<"set next block"<<endl;
    this->drawNextBlock();
   cout<<"finished drawing block"<<endl;
    if (shouldClearBlind) clearBlind();
   cout<<"cleared blind if necessary"<<endl;
    recalculateGrid();
   cout<<"finished recalcultaeGrid"<<endl;
    // some logic for determining correct notify will need to go here
    //if (getIsDecorated())setIsDecorated(false);
}

// assign the point pointer to currentBlock, can
// be used in moveLeft, moveRight
// target::block
void Player::setCurrentBlock(char type) {
    unique_ptr<AbstractBlock>tmp{level->generateBlock(type)};
    currentBlock->removeAllPoint();
    currentBlock = std::move(tmp);
    currentBlock->initialize(this);
}
int Player::getHighScore(){
    return highestScore;
}
void Player::setHighScore(int hi) {
    highestScore = hi;
    if(w!=nullptr) game->drawBigString(10, 24, to_string(highestScore), this->no);
}
