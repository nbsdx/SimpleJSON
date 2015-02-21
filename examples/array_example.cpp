
#include "json.hpp"
#include <iostream>

using json::JSON;
using namespace std;

int main()
{
    JSON array;

    array[2] = "Test2";
    cout << array << endl;
    array[1] = "Test1";
    cout << array << endl;
    array[0] = "Test0";
    cout << array << endl;
    array[3] = "Test4";
    cout << array << endl;

    // Arrays can be nested:
    JSON Array2;

    Array2[2][0][1] = true;

    cout << Array2 << endl;
}
