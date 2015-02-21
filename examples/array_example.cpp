
#include "json.hpp"
#include <iostream>

using namespace json;
using namespace std;

int main()
{
    JSON array;

    array[2] = "Test2";
    cout << array.ToString() << endl;
    array[1] = "Test1";
    cout << array.ToString() << endl;
    array[0] = "Test0";
    cout << array.ToString() << endl;
    array[3] = "Test4";
    cout << array.ToString() << endl;
}
