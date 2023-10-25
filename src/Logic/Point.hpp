#ifndef _POINTHPP_
#define _POINTHPP_
class Point{
private:
public:
    int x;
    int y;
    Point() : x{0}, y{0} {}
    Point(int _x, int _y) : x{_x}, y{_y} {}
    int getX() {return x;}
    void setX(int _x) { x = _x; }
    int getY() {return y;}
    void setY(int _y) { y = _y; }

    bool operator==(Point p)
    {
        return p.getX() == this->getX() && p.getY() == this->getY();
    }
    bool operator!=(Point p)
    {
        return !(p == *this);
    }
};

#endif