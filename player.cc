#include "player.h"
#include "abstractPlayer.h"
#include "game.h"
using namespace std;

// initialzie the player, draw in the constructor, save window pointer
Player::Player(Game *game) : AbstractPlayer{game} {}
// player's operation
// check if it is possible to level up/down
void Player::setLevel(int level){}
// check if it's movable, then call block's method to move
void Player::moveLeft(int step){}
void Player::moveRight(int step){}
void Player::moveDown(int step){}
void Player::rotateClockwise(){}
void Player::rotateCounterClockwise(){}
// add the points of blocks to grid, update the block in drop(), 
void Player::drop(){}
// assign the point pointer to currentBlock, can
// be used in moveLeft, moveRight
// target::block
void Player::putPoint(int x, int y){}
void Player::removePoint(int x, int y){}
// display
std::string Player::printLine(){}
void Player::setCurrentBlock() {}

void Player::setRandom() {} 