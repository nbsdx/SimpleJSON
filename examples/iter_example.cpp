
#include "json.hpp"
#include <iostream>

using json::JSON;
using namespace std;

int main()
{
    JSON array = JSON::Make( JSON::Class::Array );
    JSON obj   = JSON::Make( JSON::Class::Object );

    array[0] = "Test0";
    array[1] = "Test1";
    array[2] = "Test2";
    array[3] = "Test3";

    for( auto i : array.ArrayRange() )
        std::cout << "Value: " << i << "\n";

    obj[ "Key0" ] = "Value1";
    obj[ "Key1" ] = array;
    obj[ "Key2" ] = 123;

    for( auto i : obj.ObjectRange() )
        std::cout << "Key: " << i.first << ", Value: " << i.second << "\n";

}
