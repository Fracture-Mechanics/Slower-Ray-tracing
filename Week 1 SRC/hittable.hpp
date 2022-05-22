#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "Ray.hpp"

#include <memory>
#include <vector>
using namespace std;

class Material;

struct hit_record {
    Point p;
    Vec norm;
    shared_ptr<Material> mat_ptr;
    double t;
    bool front_face;

    inline void set_face_normal(const Ray& r, const Vec& outward_normal) {
    // determines if ray is coming from the front face or is inside the sphere hitting the sphere again, updates struct
        front_face = dot(r.direction(), outward_normal) < 0; // front face is true if dot product is negative meaning angles greater than 90, our normal points from the sphere center to the sphere edges 
        norm = front_face ? outward_normal : -outward_normal; // if front face is false make normal negative, meaning normal points in to the center
    }
};

class Hittable {
    public:
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

class Sphere : public Hittable {
    public:
        Point center;
        double radius;// declartion 
        shared_ptr<Material> mat_ptr;

        Sphere(Point cen, double r, shared_ptr<Material> m) : center(cen), radius(r), mat_ptr(m) {}; // init lst

        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const { // func 
            Vec oc = r.origin() - center;
            auto a = r.direction().length_squared();
            auto half_b = dot(oc, r.direction());
            auto c = oc.length_squared() - radius*radius;

            auto discriminant = half_b*half_b - a*c;
            if (discriminant < 0) return false;
            auto sqrtd = sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range.
            auto root = (-half_b - sqrtd) / a; //calculate the root case one subtract square root, 
            if (root < t_min || t_max < root) { // if we subtract the discrimint and we get a negative we will do addition instead 
                root = (-half_b + sqrtd) / a; // we still need t max in there because half_B might be negative so becomes positive 
                if (root < t_min || t_max < root)
                    return false;// does not meet requirement for t_min/t_max no intersect with sphere
            }

            rec.t = root;
            rec.p = r.at(rec.t); // the entire ray including origin and direction
            Vec outward_normal = (rec.p - center) / radius; // gives a unit length normal because p - center should be containined within the sphere
            rec.set_face_normal(r, outward_normal); // stores normal(of refeclection ) and if ray front face in the hit_record struct
            rec.mat_ptr = mat_ptr;

            return true;
        }
};

class Hittable_list : public Hittable {
    public:
        //consturctor 
        vector<shared_ptr<Hittable>> objects; // init vector of shared pointer of datatype Hittable vector name is Objects
        Hittable_list() {}
        Hittable_list(shared_ptr<Hittable> object) { add(object); }// for a hittable_list pass in a object of the Hittable class(child included)
        // add function is called upon init, adds the object passed in during init to the vector called objects 
        void clear() { objects.clear(); }// clears the entire vector
        void add(shared_ptr<Hittable> object) { objects.push_back(object); } // push back is a method for vector that adds to it 

        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
            hit_record temp_rec;// init struct 
            bool hit_anything = false; // default is false, will update later
            auto closest_so_far = t_max;

            for (const auto& object : objects) { // for item in vector, each item is a sphere
                if (object->hit(r, t_min, closest_so_far, temp_rec)) {// calls the hit method of the sphere class 
                    hit_anything = true;// if hit somthing then sphere hit will return true, 
                    closest_so_far = temp_rec.t;// from the sphere hit function that updated rec for us 
                    rec = temp_rec; //updates rec passed in in this hittable list function 
                }       
            }

        return hit_anything;// if hit we need to return a bool so we know which ray to shade 

        }      
};

#endif