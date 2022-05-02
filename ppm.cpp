// write to ppm file
#include<iostream>
using namespace std;

int main() {
    // creates a image file of colour gradients// 
    
    //set image width and height
    const int img_width = 256;
    const int img_height = 256;
    //writing standard P3 ppm header 
    cout << "P3 " <<img_width << ' ' <<img_height<< "\n255\n";
    //loop start from max height and min width 
    for (int i = img_height-1; i>= 0; i-- ){
        for (int j = 0; j < img_width; j++ ){

            auto r = double(j) / (img_width-1); // auto lets complier deduce the var type
            auto g = double(i) / (img_height-1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r); //static_cast is used for type conversion
            int ig = static_cast<int>(255.999 * g); // here we convert double to int 
            int ib = static_cast<int>(255.999 * b);
            
            if (j<255){ // 0 to 255 is 256 long
                //cout<< j<< ' ';
                cout << ir << ' ' << ig << ' ' << ib <<' ';
            }

            else if (j==255) {
                //cout<<j<<' '<<'\n';
                cout << ir << ' ' << ig << ' ' << ib << '\n';
            }
            // file output, in cmd cd to current dir \ program.exe > save_file.ppm
        }
    }
}