#ifndef MATERIAL_H
#define MATERIAL_H

#include "Ray.hpp"

struct hit_record;

class Material {
    public:
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Colour& reduction, Ray& scattered) const = 0;
};

class Lambertian : public Material {
    public:  
        Colour albedo;// declare
        Lambertian(const Colour& a) : albedo(a) {} // init

        virtual bool scatter(const Ray& r_in, const hit_record& rec, Colour& reduction, Ray& scattered) const override {
            auto scatter_direction = rec.norm + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.norm;
            }

            scattered = Ray(rec.p, scatter_direction);
            reduction = albedo;
            return true;
        }
};  

class Metal : public Material {
    public: 
        Colour albedo; //declaration 
        double fuzz;
        // if fuzz is 0 meaning fully reflective metal, else, fuzz factor defult of 1 because 1*random reflection   
        Metal(const Colour& a, double f) : albedo(a), fuzz(f<1 ? f : 1) {}// init

        virtual bool scatter(
            const Ray& r_in, const hit_record& rec, Colour& reduction, Ray& scattered) const override {
            Vec reflected = reflect(unit_vector(r_in.direction()), rec.norm);
            scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            reduction = albedo;
            return (dot(scattered.direction(), rec.norm) > 0);
        }

     
};

class Dielectric : public Material {
    
    private:
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }

    public:
        double ir;// Index of Refraction

        Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const Ray& r_in, const hit_record& rec, Colour& reduction, Ray& scattered) const override {
            reduction = Colour(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

            Vec unit_direction = unit_vector(r_in.direction());
            
            double cos_theta = fmin(dot(-unit_direction, rec.norm), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            Vec direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, rec.norm);
            else
                direction = refract(unit_direction, rec.norm, refraction_ratio);

            scattered = Ray(rec.p, direction);
            return true;
        }
         
};

#endif