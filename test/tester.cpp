#include "json.hpp"
#include <iostream>
#include <fstream>
#include <streambuf>

using namespace std;
using json::JSON;

void usage( char *name )
{
    std::cout << "Usage: " << name << " input.json output.json\n";
    exit( 1 );
}

/**
 *  Simple testing program that takes an input
 *  JSON file, loads it, and then dumps it the
 *  given output file.
 */
int main( int argc, char **argv )
{
    if( argc != 2 )
        usage( argv[0] );

    string contents;
    ifstream input( argv[1] );
    input.seekg( 0, ios::end );
    contents.reserve( input.tellg() );
    input.seekg( 0, ios::beg );

    contents.assign( (istreambuf_iterator<char>( input )),
                     istreambuf_iterator<char>() );

    JSON obj = JSON::Load( contents );

    //ofstream output( argv[2] );
    cout << obj << endl;
}
