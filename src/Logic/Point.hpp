class Point{
private:
    static int numberOfPoints;
    int x;
    int y;
public:
    const int id;
    Point(int _x, int _y) : x{_x}, y{_y}, id{numberOfPoints++} {}
    int getX() {return x;}
    void setX(int _x) { x = _x; }
    int getY() {return y;}
    void setY(int _y) { x = _y; }
};

int Point::numberOfPoints = 0;