// write to ppm file
#include "Colour.hpp"
#include "hittable.hpp"
#include "Camera.hpp"
#include "Material.hpp"

Colour ray_colour(const Ray& r, const Hittable& world, int depth) {
    hit_record rec;
    // If we've exceeded the ray bounce limit "depth", no more light is gathered.
    if (depth <= 0)
        return Colour(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        //now for metals:
        Ray scattered;
        Colour reduction;
        if (rec.mat_ptr->scatter(r, rec, reduction, scattered)){
            return reduction * ray_colour(scattered, world, depth-1); 
        }

        return Colour(0,0,0);

    }

    Vec unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction[1] + 1.0);
    return (1.0-t) * Colour(1.0, 1.0, 1.0) + t * Colour(0.5, 1, 1.0);
}


Hittable_list random_scene() {
    Hittable_list world;

    auto ground_material = make_shared<Lambertian>(Colour(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Colour::random() * Colour::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Colour::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Colour(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Colour(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point(4, 1, 0), 1.0, material3));

    return world;
}

int main() {

    //set image width and height
    const auto aspect_ratio = 3.0 / 2.0;
    const int img_width = 400;
    const int img_height = static_cast<int>(img_width / aspect_ratio);
    const int samples_per_pixel = 100; // free to change
    const int max_depth = 50;
    // World
    auto world = random_scene();

    // Camera
    Point lookfrom(13,2,3);
    Point lookat(0,0,0);
    Vec vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);   

    //writing standard P3 ppm header 
    cout << "P3 " <<img_width << ' ' <<img_height << "\n255\n";

    //loop start from max height and min width 
    for (int i = img_height-1; i>= 0; i-- ){
        for (int j = 0; j < img_width; j++ ){
            //this 2 lines below just allows us to move top to bottom and left to right dividing viewport(unit length) in to img_width/height amount of
            //pixles. so in our case start from top left move to right and then go down. since lower left corner + max height
            // case i=0 v does not change horizontal , case i/ssng = 1, end of horizontal the max, everything 
            //else just moves left to right., we are dividing view port of 2 in to img_width amount of slices each represneting a pixle
            Colour pixel_colour(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) { // this loop means we send x amount of rays in to one pixle and take the average

                auto u = (j + random_double()) / (img_width-1);
                auto v = (i + random_double()) / (img_height-1);
                Ray r = cam.get_ray(u, v);// just gets a ray in vector form 
            
                pixel_colour += ray_colour(r, world, max_depth); // add to pixle_colour
            }

            write_colour(cout, pixel_colour, samples_per_pixel); // here we take the avarage of pixle colours
  
            // file output, in cmd cd to current dir \ program.exe > save_file.ppm
        }
    }
}
//this func gets the colour of each pixle from the ray at view point
