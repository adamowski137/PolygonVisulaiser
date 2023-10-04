#include <vector>
#include "Point.hpp"

class Polygon{
private:
    int count;
    std::vector<Point> verticies;
public:
    Polygon() : verticies{}, count{0} {}
    void addVertex(Point p);
    void deleteVertex(Point p);
};