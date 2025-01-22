#include <iostream>
#include <thread>
#include <chrono>
using namespace std;


int main()
{
    std::cout << "Hello World!\n";
    for (int i = 0; i < 1000000; i++) {
        cout << i << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    std::cout << "end;";
}
