#include "poly_algo.h"

int main(void) {
    
    Polynomial a,b,c;
    std::cout << "Initialization: \n" << a << std::endl;
    a.fill(10);
    std::cout << "Fill the value with 10: \n" << a << std::endl;

    a.fill(Q - 10);
    std::cout << "Fill the value with Q - 10: \n" << a << std::endl;

    a.reduced();
    std::cout << "Reduced the polynomial: \n" << a << std::endl;

    a.negative_add_Q();
    std::cout << "Negative add Q: \n" << a << std::endl;

    b.fill(10); c.fill(10);
    a = b + c;
    std::cout << "Adding operator in a: \n" << a << std::endl;

    a = b + c;
    std::cout << "Adding operator in a: \n" << a << std::endl;

    a+= b;
    std::cout << "Adding operator in a: \n" << a << std::endl;
    
    a.fill(1);
    a.NTT();
    std::cout << "Result of NTT translation: \n" << a << std::endl;

    a.invNTT();
    a.reduced();
}