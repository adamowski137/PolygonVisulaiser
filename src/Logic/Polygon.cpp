#include "Polygon.hpp"
#include "Point.hpp"
#include <math.h>
#include <iostream>
#include <algorithm>


void Polygon::addVertex(int x, int y)
{
    Point p = Point(x, y);
    verticies.push_back(p);
    verticalLines[&verticies[count]] = std::set<Point*>();
    horizontalLines[&verticies[count]] = std::set<Point*>();
    count++;
}

bool Polygon::deleteVertex(Point p)
{
    for(int i = 0; i < count; i++)
    {
        if(verticies[i].getX() == p.getX() && verticies[i].getY() == p.getY())
        {
            verticies.erase(verticies.begin() + i);
            horizontalLines.clear();
            verticalLines.clear();
            count--;
            return true;
        }
    }
    return false;
}

void Polygon::getXCordinates(short int tmp[])
{
    for (int i = 0; i < count; i++)
    {
        tmp[i] = verticies[i].getX();
    }
}

void Polygon::getYCordinates(short int tmp[])
{
    for (int i = 0; i < count; i++)
    {
        tmp[i] = verticies[i].getY();
    }
}

bool Polygon::isPointInside(int x, int y)
{
    int interceptions = 0;
    Point start{0,0};
    Point current{x, y};
    Point cur = verticies[0];
    Point next = verticies[count - 1];
    
    if (doIntersect(start, current, cur, next))
        interceptions++;

    for(int i = 0; i < count - 1; i++)
    {
        cur = verticies[i];
        next = verticies[i + 1];
        if (doIntersect(start, current, cur, next))
            interceptions++;        
    }
    
    return ((interceptions % 2) == 1);
}

bool Polygon::onSegment(Point p, Point q, Point r) 
{ 
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && 
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y)) 
       return true; 
  
    return false; 
} 
  
int Polygon::orientation(Point p, Point q, Point r) 
{ 
    int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0; 
  
    return (val > 0)? 1: 2; 
} 

bool Polygon::doIntersect(Point p1, Point q1, Point p2, Point q2) 
{ 
    // Find the four orientations needed for general and 
    // special cases 
    int o1 = orientation(p1, q1, p2); 
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 
  
    // General case 
    if (o1 != o2 && o3 != o4) 
        return true; 
  
    // Special Cases 
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1 
    if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
  
    // p1, q1 and q2 are collinear and q2 lies on segment p1q1 
    if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
  
    // p2, q2 and p1 are collinear and p1 lies on segment p2q2 
    if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
  
     // p2, q2 and q1 are collinear and q1 lies on segment p2q2 
    if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
  
    return false; // Doesn't fall in any of the above cases 
} 

void Polygon::move(int offsetX, int offsetY)
{
    for(int i = 0; i < count; i++)
    {
        Point p{offsetX + verticies[i].getX(), offsetY + verticies[i].getY()};
        verticies[i] = p;
    }
}

Point* Polygon::getClickedVertex(int x, int y, int epsilon)
{
    for (int i = 0; i < count; i++)
    {
        if(std::abs(verticies[i].getX() - x) < epsilon && std::abs(verticies[i].getY() - y) < epsilon)
        {
            return &verticies[i];
        }
    }
    return nullptr;
}

Polygon Polygon::getShiftedPolygon(int n)
{
    Polygon newPolygon{};

    int index = 0;
    for (int i = 0; i < count; i++)
    {
        if(verticies[i].getY() < verticies[index].getY())
        {
            index = i;
        }
    }
    double lineParameters[count][3];
    int constant = 1;
    int curr = (index + 0) % count;
    int prev = (curr + count - 1) % count;
    int next = (curr + 1) % count;
    if(
        crossProduct(verticies[prev].getX(), verticies[prev].getY(), 
        verticies[curr].getX(), verticies[curr].getY(), 
        verticies[next].getX(), verticies[next].getY()) == -1)
    {
        constant = -1;
    }
    for(int i = 0; i < count; i++)
    {
        curr = (index + i) % count;
        prev = (curr + count - 1) % count;
        next = (curr + 1) % count;
        double A, B, C;
        int x1 = verticies[curr].getX();
        int y1 = verticies[curr].getY();
        int x2 = verticies[next].getX();
        int y2 = verticies[next].getY();
        
        A = (y2 - y1);
        B = (x2 - x1);
        C = x2 * y1 - x1 * y2;

        double shiftY = constant*n*B;
        double shiftX = constant*n*A;

        if(crossProduct(x1, y1, x2, y2, x1, y1 - n) == -1)
        {
            shiftY *= -1;
        }
        if(crossProduct(x1, y1, x2, y2, x1 + n, y1) == -1)
        {
            shiftX *= -1;
        }
        double newC = shiftX + shiftY + C;

        lineParameters[i][0] = A;
        lineParameters[i][1] = B;
        lineParameters[i][2] = newC;
        
    }

    for(int i = 0; i < count; i++)
    {
        int j = ((i + 1) % count);
        double x = 0, y = 0;
        double A1 = lineParameters[i][0];
        double B1 = lineParameters[i][1];
        double C1 = lineParameters[i][2];

        double A2 = lineParameters[j][0];
        double B2 = lineParameters[j][1];
        double C2 = lineParameters[j][2];

        double det = A1 * B2 - A2 * B1;

        if (std::abs(det) > 0.001) 
        {
            x = ((double)(C2 * B1 - C1 * B2) / det);
            y = ((double)(A1 * C2 - A2 * C1) / det);
            newPolygon.addVertex(x, y);
        }
    }

    newPolygon.borderColor[0] = 0;
    newPolygon.borderColor[1] = 255;
    newPolygon.borderColor[2] = 0;
    newPolygon.borderColor[3] = 255;
    return newPolygon;
}

int Polygon::crossProduct(double sx, double sy, double mx, double my, double ex, double ey) {
    double vector1X = mx - sx;
    double vector1Y = my - sy;

    double vector2X = ex - mx;
    double vector2Y = ey - my;

    double crossProduct = vector1X * vector2Y - vector1Y * vector2X;
     
    if (crossProduct > 0.1) {
        return 1; // Skręt w lewo
    } else if (crossProduct < -0.1) {
        return -1; // Skręt w prawo
    } else {
        return 0; // Brak skrętu (równoległe)
    }
}

std::pair<Point*, Point*>* Polygon::getClickedEdge(int x, int y, int epsilon)
{
    for(int i = 0; i < count; i++)
    {
        int curr = i;
        int next = (curr + 1) % count;
        long int A, B, C;
        long int x1 = verticies[curr].getX();
        long int y1 = verticies[curr].getY();
        long int x2 = verticies[next].getX();
        long int y2 = verticies[next].getY();
        if(x < std::min(x1, x2) || x > std::max(x1, x2) || y < std::min(y1, y2) || y > std::max(y1, y2))
        {
            continue;
        }
        
        A = (y2 - y1);
        B = (x1 - x2);
        C = x2 * y1 - x1 * y2;
        if (std::abs(A * x + B * y + C)/std::sqrt(A*A + B*B) < epsilon)
        {

            return new std::pair<Point*, Point*>{&verticies[i], &verticies[(i + 1) % count]};
        }
    }
    return nullptr;
}

void Polygon::insertOnEdge(Point* p1, Point* p2)
{
    int insertAt;
    for(int i = 0; i < count; i++)
    {
        if(p1->getX() == verticies[i].getX() && p1->getY() == verticies[i].getY())
        {
            insertAt = i;
            break;
        }
    }
    if(p2->getX() == verticies[(insertAt + 1) % count].getX() && p2->getY() == verticies[(insertAt + 1) % count].getY())
    {
        insertAt = (insertAt + 1) % count;
    }
    

    auto t1 = verticalLines[p1].find(p2);
    auto t2 = verticalLines[p2].find(p1);

    verticalLines.clear();
    horizontalLines.clear();

    int x1 = p1->getX();
    int y1 = p1->getY();
    int y2 = p2->getY();
    int x2 = p2->getX();


    int dx = (x2 - x1)/2;
    int dy = (y2 - y1)/2;

    int x = x1 + dx;
    int y = y1 + dy;

    Point p = Point(x, y);

    verticies.insert(verticies.begin() + insertAt, p);
    count++;

    return;
}

void Polygon::setHorizontal(Point* p1, Point* p2)
{
    if(horizontalLines[p2].size() > 0 || horizontalLines[p1].size() > 0)
    {
        return;
    }
    p2->setY(p1->getY());
    horizontalLines[p2].insert(p1);
    horizontalLines[p1].insert(p2);
}

void Polygon::setVertical(Point* p1, Point* p2)
{    
    if(verticalLines[p2].size() > 0 || verticalLines[p1].size() > 0)
    {
        return;
    }
    
    p2->setX(p1->getX());
    verticalLines[p2].insert(p1);
    verticalLines[p1].insert(p2);
}

void Polygon::updateHorizontalAndVertical(Point* p)
{
    std::for_each(
        std::begin(verticalLines[p]),
        std::end(verticalLines[p]),
        [p](Point* p2) {
            p2->setX(p->getX());
        }
    );
    std::for_each(
        std::begin(horizontalLines[p]),
        std::end(horizontalLines[p]),
        [p](Point* p2) {
            p2->setY(p->getY());
        }
    );
}

std::vector<Point> Polygon::getVerticalAndHorizontalPoints()
{
    std::vector<Point> points;
    for(auto& t : verticalLines)
    {
        Point* p1 = t.first;
        for(auto& p2 : t.second)
        {
            int x = p1->getX() + (p2->getX() - p1->getX())/2;
            int y = p1->getY() + (p2->getY() - p1->getY())/2;
            points.push_back(Point(x, y));
        }
    }
    for(auto& t : horizontalLines)
    {
        Point* p1 = t.first;
        for(auto& p2 : t.second)
        {
            int x = p1->getX() + (p2->getX() - p1->getX())/2;
            int y = p1->getY() + (p2->getY() - p1->getY())/2;
            points.push_back(Point(x, y));
        }
    }

    return points;
}