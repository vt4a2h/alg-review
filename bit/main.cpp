#include <bitset>
#include <iostream>
#include <sstream>

template <class T>
void printBit(T val)
{
    std::cout << std::bitset<sizeof(val) * 8>(val) << std::endl;
}

// Insert m into n, into position starts from j ends to i
int updateBits(int n, int m, int i, int j)
{
    printBit(n);
    printBit(m);

    int allOnce = ~0;
    int left  = allOnce << (j + 1);
    int right = (1 << i) - 1;

    int mask = left | right;
    int cleared = n & mask;
    int shifted = m << i; // move m into correct position
    return cleared | shifted;
}

// Given real number between 0 and 1 (passed as a double), print binary representation or "Error"
// is number larger that 32 binary characters
std::string toBinary(double n)
{
    if (n >= 1 || n <= 0)
        return "Error";

    std::string result(".");

    while (n > 0) {
        if (result.length() >= 32)
            return "Error";

        double r = n * 2;
        if (r >= 1) {
            result.append("1");
            n = r - 1;
        } else {
            result.append("0");
            n = r;
        }
    }

    return result;
}

int main(int /*argc*/, char */*argv*/[])
{
    // 1
//    printBit(updateBits(1 << 8, 15, 2, 6));

    // 2
    std::cout << toBinary(0.101) << std::endl;

    return 0;
}
