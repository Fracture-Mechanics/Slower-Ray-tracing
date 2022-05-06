#ifndef COLOUR_HPP
#define COLOUR_HPP

#include "Vec.hpp"
#include <iostream>
using namespace std;

void write_colour(ostream& out, Colour pixel_colour) { //just a printing formatting 
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_colour[0]) << ' ' //x
        << static_cast<int>(255.999 * pixel_colour[1]) << ' ' //y
        << static_cast<int>(255.999 * pixel_colour[2]) << '\n'; //z
}


#endif