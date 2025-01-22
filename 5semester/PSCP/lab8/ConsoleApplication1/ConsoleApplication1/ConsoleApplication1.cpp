#include <iostream>


void MyFunction(int value, void (*cb)(int)) {
    std::cout << "main logic: " << value << std::endl;
    cb(value * 2);
}

void MyCallback(int result) {
    std::cout << "callback result: " << result << std::endl;
}

int main() {
    MyFunction(10, MyCallback);
    return 0;
}
