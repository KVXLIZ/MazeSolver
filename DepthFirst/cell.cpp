#include <iostream>
#include <tuple>
#include <list>
using namespace std;

struct color
{
    int red;
    int green;
    int blue;
};

class Cell{
    private:
        color clr;
        bool wall;
        tuple<int, int> coordinates;
    public:
        bool isWall() {return wall;}
        tuple<int, int> getXY() {return coordinates;}
        void setXY(int i, int j) {coordinates = make_tuple(j, i);}
        color getColor() {return clr;}
        void setColor(color c) {clr = c; wall = c.red == 0 && c.green == 0 && c.blue == 0;}
    
};