#ifndef _POLYGON_HPP_
#define _POLYGON_HPP_

#include <vector>
#include "Point.hpp"

class Polygon{
private:
    int count;
    std::vector<Point> verticies;
    bool onSegment(Point p, Point q, Point r);
    int orientation(Point p, Point q, Point r);
    bool doIntersect(Point p1, Point q1, Point p2, Point q2);
public:
    Polygon() : verticies{}, count{0} {}
    void addVertex(int x, int y);
    void deleteVertex(Point p); 
    void getXCordinates(short int tmp[]);
    void getYCordinates(short int tmp[]);
    int getCount() {return count;}
    bool isPointInside(int x, int y);
    void move(int offsetX, int offsetY);
};

#endif