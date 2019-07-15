// g++ test.cpp --std=c++11
#include <functional>
#include <iostream>

using namespace std;

double Combiner(double a, double b, std::function<double(double, double)> func) {
    return func(a, b);
}

double Add(double a, double b) {
    return a + b;
}

double Mult(double a, double b) {
    return a * b;
}

int main() {
    int a = Combiner(12, 13, Add);
    int b = Combiner(12, 13, Mult);
    cout << "A : " << a << "\n";
    cout << "B : " << b << "\n";
    return 0;
}