#include <iostream>
#include <cmath>

using namespace std;


class Complex {
private:
    double re, im;
public:
    Complex(double real = 0, double image = 0) {
        re = real;
        im = image;
    }
    double getRe() { return re; }
    double getIm() { return im; }
    void setRe(double real) { re = real; }
    void setIm(double image) { im = image; }

    double mod() { return  sqrt(re * re + im * im); }

    Complex operator -  ();  // Комплексно-сопряженное (Перегрузка вместо метода)
    Complex operator +  (const Complex&);
    Complex operator += (const Complex&);
    Complex operator -  (const Complex&);
    Complex operator -= (const Complex&);
    Complex operator *  (const Complex&);
    Complex operator *= (const Complex&);
    Complex operator /  (const Complex&);
    Complex operator /= (const Complex&);
    bool operator == (const Complex&);
    bool operator != (const Complex&);

    friend istream& operator>> (istream&, Complex&);
    friend ostream& operator<< (ostream&, const Complex&);
};


bool Complex::operator == (const Complex& c1) {
    return (re == c1.re && im == c1.im);
}


bool Complex::operator != (const Complex& c1) {
    return !(*this == c1);
}


Complex Complex::operator - () {
    Complex result;
    result.re = re;
    result.im = -im;
    return result;
}


Complex Complex::operator + (const Complex& c1) {
    Complex result;
    result.re = re + c1.re;
    result.im = im + c1.im;
    return result;
}


Complex Complex::operator += (const Complex& c1) {
    *this = *this + c1;
    return *this;
}


Complex Complex::operator - (const Complex& c1) {
    Complex result;
    result.re = re - c1.re;
    result.im = im - c1.im;
    return result;
}


Complex Complex::operator -= (const Complex& c1) {
    *this = *this - c1;
    return *this;
}


Complex Complex::operator * (const Complex& c1) {
    Complex result;
    result.re = re * c1.re - im * c1.im;
    result.im = re * c1.im + im * c1.re;
    return result;
}


Complex Complex::operator *= (const Complex& c1) {
    *this = *this * c1;
    return *this;
}


Complex Complex::operator / (const Complex& c1) {
    Complex result;
    double temp = (c1.re * c1.re + c1.im * c1.im);

    if (temp == 0) {
        result.re = 0;
        result.im = 0;
        cout << "Div is impossible!!!\n";
        return result;
    }

    result.re = (re * c1.re + im * c1.im) / temp;
    result.im = (c1.re * im - re * c1.im) / temp;

    return result;
}


Complex Complex::operator /= (const Complex& c1) {
    *this = *this / c1;
    return *this;
}


istream& operator>> (istream& in, Complex& c1) {
    double real, image;
    cout << "Real: ";
    in >> real;
    cout << "Image: ";
    in >> image;
    c1.re = real;
    c1.im = image;
    return in;
}

ostream& operator << (ostream& out, const Complex& c1) {
    if (c1.im < 0)
        return out << c1.re << c1.im << 'i';
    return out << c1.re << '+' << c1.im << 'i';
}


int main() {
    Complex z1(2, 3);
    Complex z2(z1);

    cout << z2 << endl;

    return 0;
}
