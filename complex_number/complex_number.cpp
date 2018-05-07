#include<stdio.h>
#include<stdlib.h>/* srand, rand */
#include<iostream>
#include<vector>
#include<time.h>
#include<algorithm>
#include<math.h>
using namespace std;

template <class T>
class complex{
        T real, image;
    public:
        complex():real(T(0.)), image(T(0.)) {}
        complex(T real_, T image_):real(real_), image(image_) {}
        complex inverse() const{
            T norm = real * real + image * image;
            //assume norm can not be 0.
            return complex(real / norm, -image / norm);
        }
        complex operator+(const complex &b){
            return complex(real + b.real, image + b.image);
        }
        complex operator-(const complex &b){
            return complex(real - b.real, image - b.image);
        }
        complex operator*(const complex &b){
            return complex(real * b.real - image * b.image, real * b.real + image * b.image);
        }
        complex operator/(const complex &b){
            //return (*this) * b.inverse(); 
            complex inversed_b = b.inverse();
            return complex(real * inversed_b.real - image * inversed_b.image, real * inversed_b.image + image * inversed_b.real);
        }
        T getreal(){
            return real;
        }
        T getimage(){
            return image;
        }
};

template <class T>
ostream& operator<<(ostream &os, complex<T> &b){
    if (b.getimage() >= T(0.))
        os << b.getreal() << "+" << b.getimage() << "i";
    else
        os << b.getreal() << "-" << -b.getimage() << "i";
    return os;
}

int main(){
    complex<double> ac(1.3, 0.5), bc(2., 0.);
    cout << ac << endl << bc << endl;
    complex<double> sum = ac + bc;
    cout << sum << endl;
    complex<double> divided = ac / bc;
    cout << divided << endl;
    //wrong syntax
    //cout << ac + bc << " " << ac - bc << " " << ac * bc << " " << ac / bc << endl;
    return 0;
}
