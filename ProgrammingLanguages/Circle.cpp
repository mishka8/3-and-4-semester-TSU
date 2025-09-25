#include <iostream>
#include <cmath>

using namespace std;

class Circle
{
    double x, y, r;
public:
    Circle()
    {
        x = 0;
        y = 0;
        r = 0;
    }

    Circle(double x1, double y1, double r1)
    {
        x = x1;
        y = y1;
        r = r1;
    }

    void input()
    {
        cout << "Введите координаты центра x, y - ";
        cin >> x >> y;
        cout << "Введите координаты центра r - ";
        cin >> r;
    }

    void output()
    {
        cout << "Координаты центра (" << x << ", " << y << ")" << endl;
        cout << "Радиус круга - " << r << endl;
    }

    double square ()
    {
        return M_PI * r * r;
    }

    bool checkpoints(double x0, double y0)
    {
        return (x - x0) * (x - x0) + (y - y0) * (y - y0) <= r * r;
    }

    Circle operator+ (double k)
    {
        return Circle(x , y, r * k);
    }

    Circle& operator+= (double k)
    {
        r*=k;
        return *this;
    }

    bool operator== (const Circle& other)
    {
        return r == other.r;
    }

    bool operator!= (const Circle& other)
    {
        return r != other.r;
    }

    friend istream& operator>> (istream& in, Circle& circle)
    {
        in >> circle.x >> circle.y >> circle.r;
        return in;
    }

    friend ostream& operator<< (ostream& out, const Circle circle)
    {
        out << "Центр (" << circle.x << ", " << circle.y << ") " << "Радиус - " << circle.r << endl;
        return out;
    }

};




int main()
{
    Circle circle_1;
    Circle circle_2;
    Circle circle_3(2, 4, 10);

    cout << "Input circle1_1 -" << endl;
    circle_1.input();

    cout << "Input circle_2 - " << endl;
    circle_2.input();

    cout << circle_3;

    return 0;
}
