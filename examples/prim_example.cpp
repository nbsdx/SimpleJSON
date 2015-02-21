
#include "json.hpp"
#include <iostream>
#include <ios>

using json::JSON;
using namespace std;

int main()
{
    JSON obj;

    obj = true;
    cout << "Value: " << boolalpha << obj.ToBool() << endl;

    obj = "Test String";
    cout << "Value: " << obj.ToString() << endl;

    obj = 2.2;
    cout << "Value: " << obj.ToFloat() << endl;

    obj = 3;
    cout << "Value: " << obj.ToInt() << endl;
    
}
