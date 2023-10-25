#ifndef __STATEHPP__
#define __STATEHPP__

#include "Window.hpp"

enum ApplicationState{
    CREATE,
    MOVE,
    VERTICAL,
    HORIZONTAL,
    DELETE, 
    HULL
};

class State {
public:
    ApplicationState applicationState;
    std::vector<Polygon> figures;
    Polygon current;
    Polygon* draggedPolygon;
    Point* draggedVertex;
    std::pair<Point*, Point*>* draggedEdge;
    Polygon* hullEditPolygon;

    bool startNewFigure;
    bool leftButtonDown;
    bool hull;

    bool figuresChanged;

    int figureStartX;
    int figureStartY;
    int prevX;
    int prevY;
    int offsetX;
    int offsetY;

    unsigned int* currentHull;
    
    State()
    {
        startNewFigure = true;
        applicationState = CREATE;
        leftButtonDown = false;
        draggedPolygon = nullptr;
        draggedVertex = nullptr;
        draggedEdge = nullptr;
        currentHull = nullptr;
        hull = false;

        Polygon p1{};
        p1.addVertex(500, 700);
        p1.addVertex(500, 500);
        p1.addVertex(700, 500);
        p1.addVertex(700, 700);

        p1.borderColor[0] = 255;
        p1.borderColor[1] = 100;
        p1.borderColor[2] = 100;
        p1.borderColor[3] = 255;

        figures.push_back(p1);

        p1 = Polygon{};
        p1.addVertex(606, 514);
        p1.addVertex(1166, 323);
        p1.addVertex(1228, 845);
        p1.addVertex(806, 714);
        p1.addVertex(1004, 556);
        

        p1.borderColor[0] = 255;
        p1.borderColor[1] = 100;
        p1.borderColor[2] = 100;
        p1.borderColor[3] = 255;

        figures.push_back(p1);
    }
};

#endif