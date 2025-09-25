#include <iostream>
#include <cmath>
#include <sstream>
#include <cstring>
using namespace std;

class FreeVector {
    double x, y, z;
    
    public:
        FreeVector(double xi = 0, double yi = 0, double zi = 0): x(xi), y(yi), z(zi) {}
        
        double getX() const {return x;}
        void setX(double xi) {x = xi;}
        double getY() const {return y;}
        void setY(double yi) {y = yi;}
        double getZ() const {return z;}
        void setZ(double zi) {z = zi;}
            
        double VectorLength() const {
            return sqrt(x*x+y*y+z*z);
        }
        
        double VectorLength(const FreeVector &other){
            return sqrt(pow((other.x-x), 2)+pow((other.y-y), 2)+pow((other.z-z), 2));
        }
        
        bool operator == (const FreeVector &other) const { 
            return (x == other.x) && (y == other.y) && (z == other.z);
        }
        
        bool operator != (const FreeVector &other) {
            return !(*this == other);
        }
        
        FreeVector operator + (const FreeVector &other){
            return FreeVector(x + other.x, y + other.y, z + other.z);
        }
        
        FreeVector operator += (const FreeVector &other){
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }
        
        FreeVector operator - (const FreeVector &other){
            return FreeVector(x - other.x, y - other.y, z - other.z);
        }
        
        FreeVector operator -= (const FreeVector &other){
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }
        
        double operator * (const FreeVector &other){
            double result = x*other.x + y*other.y + z*other.z;
            return result;
        }
        
        friend istream& operator >> (istream&is, FreeVector &other) {
            is >> other.x >> other.y >> other.z;
            return is;
        }
        friend ostream& operator << (ostream&is, const FreeVector &other){
            is << "(" << other.x << "; " << other.y<< "; " << other.z << ")";
            return is;
        }

};
int main() {
   FreeVector vec1 (1, 2, 3);
   FreeVector vec2 (-3, -2 ,-1);
   cout << vec1*vec2;
}
