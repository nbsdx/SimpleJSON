
#include "../json.hpp"
#include <iostream>

using namespace std;
using json::JSON;

int main()
{
    JSON Int = JSON::Load( " 123 " );
    JSON Float = JSON::Load( " 123.234 " );
    JSON Str = JSON::Load( "\"String\"" );
    JSON EscStr = JSON::Load( "\" \\\"Some\\/thing\\\" \"" );
    JSON Arr = JSON::Load( "[1,2, true, false,\"STRING\", 1.5]" );
    JSON Obj = JSON::Load( "{ \"Key\" : \"StringValue\","
                           "   \"Key2\" : true, "
                           "   \"Key3\" : 1234, "
                           "   \"Key4\" : null }" );
    
    cout << Int << endl;
    cout << Float << endl;
    cout << Str << endl;
    cout << EscStr << endl;
    cout << Arr << endl;
    cout << Obj << endl;

}
