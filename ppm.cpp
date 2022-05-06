// write to ppm file
#include "Colour.hpp"
#include "Vec.hpp"
#include "Ray.hpp"

#include<iostream>
using namespace std;

Colour ray_colour(const Ray& r);

int main() {

    //set image width and height
    const auto aspect_ratio = 16.0 / 9.0;
    const int img_width = 400;
    const int img_height = static_cast<int>(img_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = Point(0, 0, 0); //where we look at the image
    auto horizontal = Vec(viewport_width, 0, 0);
    auto vertical = Vec(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - Vec(0, 0, focal_length);

    //writing standard P3 ppm header 
    cout << "P3 " <<img_width << ' ' <<img_height<< "\n255\n";

    //loop start from max height and min width 
    for (int i = img_height-1; i>= 0; i-- ){
        for (int j = 0; j < img_width; j++ ){

            auto u = double(j) / (img_width-1);
            auto v = double(i) / (img_height-1);
            Ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            Colour pixel_colour = ray_colour(r);

            //Color pixel_colour(double(i)/(img_height-1), double(j)/(img_width-1), 0.25);
            write_colour(cout, pixel_colour);
  
            // file output, in cmd cd to current dir \ program.exe > save_file.ppm
        }
    }
}
//this func gets the colour of each pixle from the ray at view point
Colour ray_colour(const Ray& r) { // r is the ray
    Vec unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction[1] + 1.0); //unit_direction y
    return (1.0-t) * Colour(1.0, 1.0, 1.0) + t * Colour(0.5, 0.7, 1.0);
}