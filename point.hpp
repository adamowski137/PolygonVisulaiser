class Point{
private:
    int x;
    int y;
public:
    Point(int _x, int _y) : x{_x}, y{_y} {}
    int getX() {return x;}
    void setX(int _x) { x = _x; }
    int getY() {return y;}
    void setY(int _y) { x = _y; }
};