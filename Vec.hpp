#ifndef VEC_HPP
#define VEC_HPP

#include<cmath>
#include<iostream>
using namespace std;
/* operator overloading for vector operations */

class Vec{
    public :
        //constructor using member intializer list
        double e[3]; //declaring array 
        Vec() : e{0,0,0} {} // default parameters
        Vec(double x, double y, double z) : e{x, y, z} {}// member initilizer list

        Vec operator-() const {return Vec(-e[0], -e[1], -e[2]); } // negative of self

        double operator[](int i ) const {return e[i]; } // to index the object since it is an array we cant index the object directly

        Vec& operator+=(const Vec& v) {  // only one parameter because the other one is self also pass by refrence 
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;//return current instance of object
        }

        Vec& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        Vec& operator/=(const double t) {
            return *this *= 1/t; //division is multiplication of a fraction
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; //raising each ele to the power of 2 
        }

        double length() const { //unit length
            return sqrt(length_squared());
        }
    
};
//inline for better performance 
inline std::ostream& operator<<(std::ostream &out, const Vec &v) { //cout overloading
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec operator+(const Vec &u, const Vec &v) {
    return Vec(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec operator-(const Vec &u, const Vec &v) {
    return Vec(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec operator*(const Vec &u, const Vec &v) {
    return Vec(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec operator*(double t, const Vec &v) {
    return Vec(t*v.e[0], t*v.e[1], t*v.e[2]);
} 

inline Vec operator*(const Vec &v, double t) {
    return t * v; // this overload is needed for the above because this is describing the case of num * obj and the above overload build 
    //upon that by multiplying by each element 
} 

inline Vec operator/(Vec v, double t) {
    return (1/t) * v;
}

inline double dot(const Vec &u, const Vec &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline Vec cross(const Vec &u, const Vec &v) {
    return Vec(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec unit_vector(Vec v) {
    return v / v.length();
}   

using Point = Vec; //type aliases, just another name for the same class Vec
using Colour = Vec;

#endif