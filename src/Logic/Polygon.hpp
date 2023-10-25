#ifndef _POLYGON_HPP_
#define _POLYGON_HPP_

#include <vector>
#include <map>
#include <set>
#include "Point.hpp"

class Polygon{
private:
    int count;
    std::vector<Point> verticies;
    bool onSegment(Point p, Point q, Point r);
    int orientation(Point p, Point q, Point r);
    bool doIntersect(Point p1, Point q1, Point p2, Point q2);
    bool findIntersection(double A1, double B1, double C1, double A2, double B2, double C2, double* x, double* y);
    int crossProduct(double sx, double sy, double mx, double my, double ex, double ey);
    
    std::map<Point*, std::set<Point*>> verticalLines;
    std::map<Point*, std::set<Point*>> horizontalLines;

public:
    Polygon() : verticies{}, count{0}, hullOffset{0}, borderColor{0}, verticalLines{}, horizontalLines{} {}

    void addVertex(int x, int y);
    bool deleteVertex(Point p);
    void updateHorizontalAndVertical(Point* p);
    void getXCordinates(short int tmp[]);
    void getYCordinates(short int tmp[]);
    int getCount() {return count;}
    bool isPointInside(int x, int y);
    void move(int offsetX, int offsetY);
    Point* getClickedVertex(int x, int y, int epsilon);
    std::pair<Point*, Point*>* getClickedEdge(int x, int y, int epsilon);
    Polygon getShiftedPolygon(int k);
    void insertOnEdge(Point* p1, Point* p2);
    void setHorizontal(Point* p1, Point* p2);
    void setVertical(Point* p1, Point* p2);
    std::vector<Point> getVerticalAndHorizontalPoints();
    
    unsigned int hullOffset;
    int borderColor[4];
};

#endif