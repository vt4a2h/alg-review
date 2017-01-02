#include <set>
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <map>
#include <sstream>

// to ask:
//    is ASCII or unicode?
//    is alphabet lenght specified?
//    additional space?
template <class String>
bool isUniqueChars(String const& s, size_t charsCount = 128)
{
   if (s.length() > charsCount)
      return false;

   std::vector<bool> charSet(charsCount, false);
   for (auto &&c : s)
   {
      auto code = std::size_t(c);
      if (charSet[code])
         return false;

      charSet[code] = true;
   }

   return true;
}

// to ask:
//    is case sensetive? (use ::tolower)
//    are whitespaces significant? (trim std::isspace)
template <class String>
bool permutation(String const& l, String const& r)
{
   if (l.length() != r.length())
      return false;

   auto lBegin = std::begin(l);
   auto rEnd   = std::prev(std::end(r));
   while (lBegin != std::end(l)) {
      if (*lBegin++ != *rEnd--)
         return false;
   }

   return true;
}

// We also canallocate new string if current one has not enough capacity
template <class String>
void replaceSpaces(String &s)
{
   std::size_t spaceCout = std::count_if(std::begin(s), std::end(s),
                                         [](typename String::value_type const& c){ return c == ' '; });

   if (spaceCout == 0)
      return;

   std::size_t oldLength = s.length();
   // *2 because we already have one space
   s.resize(s.length() + spaceCout * 2);

   auto lastIndex = s.length() -1;
   for (auto i = oldLength - 1; i > 0; --i)
   {
      if (s[i] == ' ') {
         s[lastIndex] = '0';
         s[lastIndex - 1] = '2';
         s[lastIndex - 2] = '%';
         lastIndex -= 3;
      } else {
         s[lastIndex--] = s[i];
      }
   }
}

// TODO: add asserts
bool isPalindromePermutation(std::string const& s)
{
   // Get codes
   std::vector<int> symbolCodes('z' - 'a' + 1, 0);
   for (auto && c : s)
      if (std::isalpha(std::tolower(c)))
         ++symbolCodes[std::tolower(c) - int('a')];

   // Check if all numbers are even (and only one can be odd)
   bool foundOdd = false;
   for (auto && count : symbolCodes) {
      if (count % 2 == 1) {
         if (foundOdd)
            return false;

         foundOdd = true;
      }
   }

   return true;
}

// One edit way. Only three operations are allowed: replace a char, insert a char and remove a char.
// Check if second string is one edited first string
namespace oew
{

//   pale,  ple  -> true
//   pales, pale -> true
//   pale,  bale -> true
//   pale,  bae  -> false

   bool oneEditReplace(std::string const& f, std::string const& s);
   bool oneEditInsert(std::string const& f, std::string const& s);

   bool oneEditWay(std::string const& f, std::string const& s)
   {
      if (f.length() == s.length())
         return oneEditReplace(f, s);
      else if (f.length() + 1 == s.length())
         return oneEditInsert(f, s);
      else if (f.length() -1 == s.length())
         return oneEditInsert(s, f);

      return false;
   }

   bool oneEditReplace(std::string const& f, std::string const& s)
   {
      bool foundDifference {false};
      for (std::size_t i = 0, total = f.length(); i < total; ++i) {
         if (f[i] != s[i]) {
            if (foundDifference)
               return false;

            foundDifference = true;
         }
      }

      return true;
   }

   // Check if you can insert a character to f to make s
   bool oneEditInsert(std::string const& f, std::string const& s)
   {
      std::size_t indexF = 0;
      std::size_t indexS = 0;

      while (indexS < s.length() && indexF < f.length()) {
         if (f[indexF] != s[indexS]) {
            if (indexF != indexS)
               return false;

            ++indexS;
         } else {
            ++indexF;
            ++indexS;
         }
      }

      return true;
   }
}

// string "compression"
std::string compress(std::string const& s)
{
   if (s.length() <= 1)
      return s;

   std::stringstream out;
   std::size_t counter = 0;
   for (std::size_t i = 0, len = s.length(); i < len; ++i) {
      ++counter;

      if (i + 1 >= len || s[i] != s[i + 1]) {
         out << s[i] << counter;
         counter = 0;
      }
   }

   std::string result = out.str();
   return result.length() < s.length() ? result : s;
}

// rotate NxN matrix by 90 degree
// layer means external part of matrix (* -- first layer, . -- second layer):
// ****
// *..*
// *..*
// ****
using Matrix = std::vector<std::vector<int>>;
void printMatrix(Matrix const& matrix);

bool rotateMatrix(Matrix & matrix)
{
   if (matrix.size() == 0 || matrix[0].size() != matrix.size())
      return false;

   for (int layer = 0; layer < int(matrix.size()) / 2; ++layer) {
      int first = layer;
      int last = matrix.size() - 1 - layer;

      for (int i = first; i < last; ++i) {
         int offset = i - first;

         int top = matrix[first][i];

         // left -> top
         matrix[first][i] = matrix[last - offset][first];

         // bottom -> left
         matrix[last - offset][first] = matrix[last][last - offset];

         // right -> bottom
         matrix[last][last - offset] = matrix[i][last];

         // top - > right
         matrix[i][last] = top;
      }
   }

   return true;
}

void printMatrix(Matrix const& matrix)
{
   for (auto && l : matrix) {
      for (auto && e : l)
         std::cout << e << " ";

      std::cout << std::endl;
   }

   std::cout << "--------" << std::endl;
}

// zero matrix (in MxN matrix, if element is 0, fill entire row and col by 0)
void setZeors(Matrix &m)
{
   if (m.size() == 0 || m[0].size() == 0)
      return;

   bool firstRowHasZero =
      std::find_if(std::begin(m[0]), std::end(m[0]), [](auto &&e){ return e == 0; }) != std::end(m[0]);
   bool firstColHasZero =
      std::find_if(std::begin(m), std::end(m), [](auto &&e){ return e[0] == 0; }) != std::end(m);

   // set zero the first row/col if element at [i][j] == 0
   for (std::size_t i = 1; i < m.size(); ++i)
      for (std::size_t j = 1; j < m[0].size(); ++j)
         if (m[i][j] == 0)
            m[i][0] = m[0][j] = 0;

   // nullify rows
   for (std::size_t i = 1; i < m.size(); ++i)
      if (m[i][0] == 0)
         std::fill(std::begin(m[i]), std::end(m[i]), 0);

   // nullify cols
   for (std::size_t j = 1; j < m[0].size(); ++j)
      if (m[0][j] == 0)
         for (auto && r : m)
            r[j] = 0;

   // nulify first r
   if (firstRowHasZero)
      std::fill(std::begin(m[0]), std::end(m[0]), 0);

   // nulify first c
   if (firstColHasZero)
      for (auto && r : m)
         r[0] = 0;
}

// string rotation check if s2 is rotation of s1 using only one call isSubstring
// rotation is actually shift
bool isRotation(std::string const& s1, std::string const& s2)
{
   if (s1.length() != s2.length() || s1.length() == 0)
      return false;

   return (s1 + s1).find(s2) != std::string::npos;
}

int main(int /*argc*/, char */*argv*/[])
{
//   std::cout << std::boolalpha << isRotation("waterbottle", "erbottlewat") << std::endl;
   return 0;
}
