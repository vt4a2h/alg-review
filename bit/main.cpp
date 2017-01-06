#include <bitset>
#include <iostream>

template <class T>
void printBit(T val)
{
    std::cout << std::bitset<sizeof(val) * 8>(val) << std::endl;
}

int main(int /*argc*/, char */*argv*/[])
{
    return 0;
}
