#ifndef RAY_HPP
#define RAY_HPP

#include "Vec.hpp"

class Ray{
    public: 
        //point is an aliases for Vec
        Point orig;
        Vec dir;
        
        //constructor for ray includes a origin and a direction
        Ray(const Point& origin, const Vec& direction): orig(origin), dir(direction) {}

        //methods just returns origin and direction we use this in main program when we want to get orig and dir
        Point origin() const  { return orig; }
        Vec direction() const { return dir; }


        Point at(double t) const {
            return orig + t*dir; //from the ray formula, returns a vector 
        }       
};


#endif