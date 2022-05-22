#ifndef COLOUR_HPP
#define COLOUR_HPP

#include "Vec.hpp"

#include <iostream>
using namespace std;

void write_colour(ostream& out, Colour pixel_colour, int samples_per_pixel) { //just a printing formatting 
    auto r = pixel_colour[0];
    auto g = pixel_colour[1];
    auto b = pixel_colour[2];

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel; // for division 
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);
    
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * fit_range(r, 0.0, 0.999)) << ' ' //x
        << static_cast<int>(256 * fit_range(g, 0.0, 0.999)) << ' ' //y
        << static_cast<int>(256 * fit_range(b, 0.0, 0.999)) << '\n'; //z
}


#endif