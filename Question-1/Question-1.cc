/// Question 1: This question requests you to fix all compile errors in the main function and any classes. Address
/// the comments given inside the code.
/// Your code should run and compile without any warnings or errors.


#include <iostream>
#include <map>
#include <vector>
#include <initializer_list>
#include <cmath>
#include <memory>

const int values[] = { 1,2,3,4,5 };
const int val_size = sizeof values / sizeof(int);



class Foo {
    std::vector<int> mElements;
public:
    Foo(std::initializer_list<int> list)
    : mElements(list) {}
};

struct person{
    int age;
    float weight;
    person(int a, double w) : age(a), weight(w) {}
};


void show_output(const int** pp)
{
    printf("%p : %p : %d", (void*)pp, (void*)*pp, **pp);
}



int main() {

    int x = -10;
    int y = 15;
    std::cout << " " << x << " " << y << std::endl;

    // print integer ratios of y:x till x < y
    // invalid ratios should print 0
    while (x < y)
    {
        //Initially i was confused about what ratios are valid or not
        //Then after seeing the floating point exception, I understood that
        //we need to fix division by zero
        if (x == 0) {
            std::cout << "ratio: 0" << std::endl;
        } else {
            std::cout << "ratio: " << (y / x) << std::endl;
        }
        x++;
        y--;
    }


    int i = 1, j = 1; // don't change values
    if ((i = !3) & (j = 1))
    {
        std::cout << "j is 1 and i is not 3\n";
    }
    else {
        std::cout << "either j is NOT 1, or i is set to 3\n";

    }

    //Knew what a map is, didn't know how it works in C++ so checking its implementation took some 
    //time. I miss python but C++ seems very robust. Had to research insert and make_pair functions 
    //too. 
    typedef std::map<int, double> valmap;
    valmap m;

    for (int i = 0; i < val_size; i++)
        m.insert(std::make_pair(values[i], std::pow(values[i], .5)));

    m.insert(std::make_pair(1, 2));

    //Pointers were basics from C, I just had to use typecasting and the error logs helped
    //me to figure that out
    int n = 1;
    int* p = &n;
    show_output((const int**)&p);

    // Initialise a person on the heap with the use of smart pointers (unique_ptr) instead.

    //Here i had to research smart pointers and understand how they work in C++
    //Since i know how memory allocation works in C so I grasped the concept but
    //didnt know how to implement
    auto ptr = std::make_unique<person>(10, 55.5);


    // Initialise with 5 integers
    // I have done 2511 and understand OOP so the concept made sense,
    // to initialise an object using the input stated in the constructor fucntion
    // I didn't know the syntax was curly braces tho.
    Foo foo {1,2,3,4,5};

    return 0;
}
