
#include "json.hpp"
#include <iostream>

using json::JSON;
using namespace std;

void dumpArrayConst( const JSON &array ) {
    for( auto &j : array.ArrayRange() )
        std::cout << "Value: " << j << "\n";
}

void dumpArray( JSON &array ) {
    for( auto &j : array.ArrayRange() )
        std::cout << "Value: " << j << "\n";
}

void dumpObjectConst( const JSON &object ) {
    for( auto &j : object.ObjectRange() )
        std::cout << "Object[ " << j.first << " ] = " << j.second << "\n";
}

void dumpObject( JSON &object ) {
    for( auto &j : object.ObjectRange() )
        std::cout << "Object[ " << j.first << " ] = " << j.second << "\n";
}

int main()
{
    JSON array = JSON::Make( JSON::Class::Array );
    JSON obj   = JSON::Make( JSON::Class::Object );

    array[0] = "Test0";
    array[1] = "Test1";
    array[2] = "Test2";
    array[3] = "Test3";

    obj[ "Key0" ] = "Value1";
    obj[ "Key1" ] = array;
    obj[ "Key2" ] = 123;


    std::cout << "=============== tests ================\n";
    dumpArray( array );
    dumpObject( obj );

    std::cout << "============ const tests =============\n";
    dumpArrayConst( array );
    dumpObjectConst( obj );
}
