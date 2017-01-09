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
std::string toBinary(float n)
{
    if (n >= 1 || n <= 0)
        return "Error";

    std::string result(".");

    while (n > 0.) {
        if (result.length() >= 32)
            return "Error";

        float r = n * 2;
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

// Given an integer value and you can flip only one bit. Find the longest sequence of 1s.
int flipBit(int a)
{
    if (~a == 0)
        return sizeof(int) * 8; // We have all 1s

    int currentLength = 0;
    int previousLength = 0;
    int maxLength = 1; // As we can flip one bit there is always sequence from 1 bit :)
    while (a != 0) {
        if ((a & 1) == 1) // Current bit is 1
            ++currentLength;
        else if ((a & 1) == 0) { // Current bit is 0
            // Update to 0 if next bit is 0, or to current length if is not
            previousLength = (a & 2) == 0 ? 0 : currentLength;
            currentLength = 0;
        }
        maxLength = std::max(previousLength + currentLength + 1, maxLength);
        a >>= 1;
    }

    return maxLength;
}

// Given a positive integer, print the next smallest and the next largest number that have
// the same number of 1 bits in their binary representation.
int getNext(int n)
{
    // Compute count of ones (c1) and count of zeroes (c0)
    int c = n;

    int c0 = 0;
    while (((c & 1) == 0) && c != 0) {
        ++c0;
        c >>= 1;
    }

    int c1 = 0;
    while ((c & 1) == 1) {
        ++c1;
        c >>= 1;
    }

    // Error: no bigger number with the same number of ones
    if (c0 + c1 == sizeof(int) - 1 || c0 + c1 == 0)
        return -1;

    int p = c0 + c1; // Position of rightmost non-traling zero

    n |= (1 << p); // Flip rightmost non-tralling zero
    n &= ~((1 << p) - 1); // Clear all bits to the riht of p
    n |= (1 << (c1 - 1)) - 1; // Insert (c1 - 1) ones to the right
    return n;
}

int getPrev(int n)
{
    int tmp = n;

    int c1 = 0;
    while ((tmp & 1) == 1) {
        ++c1;
        tmp >>= 1;
    }
    if (tmp == 0)
        return -1;

    int c0 = 0;
    while (((tmp & 1) == 0) && (tmp !=0)) {
        ++c0;
        tmp >>= 1;
    }

    int p = c0 + c1; // Position of rightmost non-traling one
    n &= ~0 << (p + 1); // Clear bits from p onwards

    int mask = (1 << (c1 + 1)) - 1; // Sequence of (c1 + 1) ones
    n |= mask << (c0 - 1);

    return n;
}

// Determine how many bits you nned to flip to convert a to b
int bitFlipRequried(int a, int b)
{
    int count = 0;
    for (int c = a ^ b; c != 0; c &= c - 1, ++count);
    return count;
}

// Swap odd and even bits
int swapOddEvenBits(int x)
{
    // 0xaaaaaaaa -- mask for odd bits for 32 bit integer
    // 0x55555555 -- for even
    return ((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1);
}

int main(int /*argc*/, char */*argv*/[])
{
    // 1
//    printBit(updateBits(1 << 8, 15, 2, 6));

    // 2
//    std::cout << toBinary(0.101) << std::endl;

    // 3
//    int a = 34213;
//    printBit(a);
//    std::cout << flipBit(a) << std::endl;

//    a = ~0;
//    printBit(a);
//    std::cout << flipBit(a) << "\t" << a << std::endl;

    // 4
//    int n = 13948;
//    printBit(n);

//    int next = getNext(n);
//    printBit(next);

//    int prev = getPrev(n);
//    printBit(prev);

//    std::cout << prev << " < " << n << " < " << next << std::endl;

    // 5
//    std::cout << bitFlipRequried(29, 15) << std::endl;

    // 6
    int n = 2324122;
    printBit(n);
    printBit(swapOddEvenBits(n));

    return 0;
}
