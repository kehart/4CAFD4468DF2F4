#include "blockX.h"
#include "abstractLevel.h"
#include "abstractPlayer.h"
using namespace std;

BlockX::BlockX(int score) : AbstractBlock{"X", score} {
//    Coordinate c1{0,0};

    auto c1 = make_pair(0, 0);
    this->positions.emplace_back(c1);
}

BlockX::~BlockX() {}

void BlockX::initialize(AbstractPlayer *p) {
    // get point pointers
//    for (Coordinate &c : this->positions) {
     for (pair<int, int> &c : positions) {
        this->addPoint(c, p);
    }
    // set points
    for (Point *p : this->points) {
        p->setType(this->type);
    }
}
