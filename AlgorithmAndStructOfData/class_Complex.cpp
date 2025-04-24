#include <iostream>
#include <cmath>
#include <stdbool.h>
#define size 255

using namespace std;


class complex
{
    double re, im;

    public:
        complex(double Re = 0, double Im = 0);

        double getRe()//посмотреть Re
        {
            return re;
        }

        void setRe(double Re)//поменять Re
        {
            re = Re;
        }

        double getIm()//посмотреть Im
        {
            return im;
        }
        void setIm(double Im)//поменять Im
        {
            im = Im;
        }

        bool operator == (const complex &);
        bool operator != (const complex &);

        double mod()//модуль
        {
            return sqrt(re * re + im * im);
        };

        operator char* ()
        {
            char* str = new char[size];

            sprintf(str, "%f + %f * i", re, im);
            return str;
        }

        complex operator -();//комплексно сопряженное число

        complex operator -(const complex &);
        complex& operator -=(const complex &);

        complex operator +(const complex &);
        complex& operator +=(const complex &);

        complex operator *(const complex &);
        complex& operator*=(const complex &);

        complex operator /(const complex &);
        complex& operator /=(const complex &);

        friend istream &operator>>(istream &st, complex &x)
        {
            st >> x.re;
            st >> x.im;
            return st;
        };

        friend ostream &operator<<(ostream &st, const complex &x)
        {
            if(x.im > 0)
                st << x.re << " + " << x.im << " * i";
            else if(x.im < 0)
                st << x.re << " - " << -1 * x.im << " * i ";
            else
                st << x.re;
            return st;
        };

};


complex:: complex(double Re, double Im)
{
    re = Re;
    im = Im;
}

bool complex:: operator== (const complex &x)
{
     if(re == x.re && im == x.im)
         return true;
     return false;
}

 bool complex:: operator!= (const complex &x)
{
     if(re != x.re || im != x.im)
         return true;
     return false;
}


complex complex:: operator -(const complex &x)
{
    complex res;
    res.re = re - x.re;
    res.im = im - x.im;
    return res;
}

complex& complex:: operator -=(const complex &x)
{
    re -= x.re;
    im -= x.im;
    return *this;
}


complex complex:: operator +(const complex &x)
{
    complex res;
    res.re = re + x.re;
    res.im = im + x.im;
    return res;
}

complex& complex:: operator +=(const complex &x)
{
    re += x.re;
    im += x.im;
    return *this;
}

complex complex:: operator *(const complex &x)
{
    complex res;
    res.re = re * x.re - im * x.im;
    res.im = re * x.im + im * x.re;
    return res;
}

complex& complex:: operator *=(const complex &x)
{
    double tmpRe = re * x.re - im * x.im;
    im = re * x.im + im * x.re;
    re = tmpRe;
    return *this;
}

complex complex:: operator /(const complex &x)
{
    complex res;
    res.re = (re * x.re + im * x.im)/(x.re * x.re + x.im * x.im);
    res.im = (im * x.re - re * x.im)/(x.re * x.re + x.im * x.im);
    return res;
}

complex& complex:: operator /=(const complex &x)
{
    double denom = x.re * x.re + x.im * x.im;
    re = (re * x.re + im * x.im) / denom;
    im = (im * x.re - re * x.im) / denom;
    return *this;
}

int main()
{
    complex a;
    complex b;
    complex c;

    cin >> a;
    cout << "number1 : " << a << endl;

    cin >> b;
    cout << "number2 : " <<  b << endl ;

    (a == b) ? cout << endl << "the same a == b\n" : cout<< endl << "different a != b\n";

    c = a - b;
    cout << "a - b = " << c << endl<< endl;

    c = a + b;
    cout << "a + b = " << c << endl << endl;

    c = a * b;
    cout << "a * b = " << c << endl << endl;

    c = a / b;
    cout << "a / b = " << c << endl << endl;

    c.mod();
    cout << "module c == " << c << endl << endl;

    a -= b;
    cout << " a -= b -> a == " << a << endl << endl;

    b += a;
    cout << " b += a -> b == " << b << endl << endl;

    a *= b;
    cout << " a *= b --> a == " << a << endl << endl;

    a /= b;
    cout << " a /= b --> a == " << a << endl << endl;



    (a != b) ? cout << "different a != b\n"  : cout << "the same a == b\n";

    cout << (char*) c;



    return 0;
}
