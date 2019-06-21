
#include "../json.hpp"
#include <iostream>
#include <cstddef>

using json::JSON;
using namespace std;

int main()
{
    JSON obj( {
        "Key", 1,
        "Key3", true,
        "Key4", nullptr,
        "Key2", {
            "Key4", "VALUE",
            "Arr", json::Array( 1, "Str", false )
        }
    } );

    cout << obj << endl;
}
