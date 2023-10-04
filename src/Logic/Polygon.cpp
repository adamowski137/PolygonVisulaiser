#include "Polygon.hpp"

void Polygon::addVertex(Point p)
{
    verticies.push_back(p);
    count++;
}

void Polygon::deleteVertex(Point p)
{
    for(auto it = verticies.begin(); it != verticies.end(); it++)
    {
        if((*it).id == p.id)
        {
            // verticies.erase(it);
            count--;
        }
    }
}