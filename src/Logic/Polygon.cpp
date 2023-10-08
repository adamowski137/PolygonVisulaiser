#include "Polygon.hpp"
#include "Point.hpp"
#include <math.h>
#include <iostream>

void Polygon::addVertex(int x, int y)
{
    verticies.push_back(Point(x, y));
    count++;
}

void Polygon::deleteVertex(Point p)
{
    for(auto it = verticies.begin(); it != verticies.end(); it++)
    {
        // if((*it).id == p.id)
        // {
        //     // verticies.erase(it);
        //     count--;
        // }
    }
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

