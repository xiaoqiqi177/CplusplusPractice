#include<stdio.h>
#include<stdlib.h>/* srand, rand */
#include<iostream>
#include<vector>
#include<time.h>
#include<algorithm>
#include<math.h>
using namespace std;

template <class T>
class Complex{
        T real, image;
    public:
        Complex():real(T(0.)), image(T(0.)) {}
        Complex(T real_, T image_):real(real_), image(image_) {}
        Complex inverse() const{
            T norm = real * real + image * image;
            //assume norm can not be 0.
            return Complex(real / norm, -image / norm);
        }
        Complex operator+(const Complex &b){
            return Complex(real + b.real, image + b.image);
        }
        Complex operator-(const Complex &b){
            return Complex(real - b.real, image - b.image);
        }
        Complex operator*(const Complex &b){
            return Complex(real * b.real - image * b.image, real * b.image + image * b.real);
        }
        Complex operator/(const Complex &b){
            Complex inversed_b = b.inverse();
            return this->operator*(inversed_b);
            //return this->Complex::operator*(inversed_b);
            //return Complex(real * inversed_b.real - image * inversed_b.image, real * inversed_b.image + image * inversed_b.real);
        }
        T getreal() const{
            return real;
        }
        T getimage() const{
            return image;
        }
        friend ostream& operator<<(ostream &os, const Complex &b){
            if (b.getimage() >= T(0.))
                os << b.getreal() << "+" << b.getimage() << "i";
            else
                os << b.getreal() << "-" << -b.getimage() << "i";
            return os;
        }
};

int main(){
    Complex<double> ac(1.3, 0.5), bc(2., 0.);
    cout << ac << endl << bc << endl;
    Complex<double> sum = ac + bc;
    cout << sum << endl;
    Complex<double> divided = ac / bc;
    cout << divided << endl;
    cout << ac + bc << " " << ac - bc << " " << ac * bc << " " << ac / bc << endl;
    return 0;
}
