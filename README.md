# SimpleJSON
Simple C++ JSON library

## About
SimpleJSON is a lightweight JSON library for exporting data in JSON format from C++. It makes heavy use of templates to make it as easy as possible for the user to make chages and generate JSON objects. SimpleJSON is packaged as a single C++ Header file "json.hpp". 

## Upcoming Features
SimpleJSON is still missing some features, which I hope to get done soon!
* Import JSON from a string.
* Convert from a JSON object to primitive.
* Allow users to assign to the next available array index to append to the array:
```
  JSON array;
  array[0] = "Value";
  array[1] = 2;
  array[2] = true;
  array[4] = 1.1; // ERROR: Element 3 hasn't been assigned to.
```
One of the biggests goals for SimpleJSON is for it to be lightweight, and small. Having complicated logic isn't bad, but it bloats the codebase in most cases. I'd like to keep things small rather than put in big features that take a ton of space.

If you run into any bugs, please submit a bug!

## Example
A larger example can be see in example.cpp, so feel free to check that out as well. 

```
#include "json.hpp"

int main() {
  JSON obj;
  // Create a new Array as a field of an Object.
  obj["array"] = JSONArray( true, "Two", 3, 4.0 );
  // Create a new Object as a field of another Object.
  obj["obj"] = JSONObject();
  // Assign to one of the inner object's fields
  obj["obj"]["inner"] = "Inside";
  
  // We don't need to specify the type of the JSON object:
  obj["new"]["some"]["deep"]["key"] = "Value";
  obj["array2"].append( false, "three" );
  
  std::cout << obj.ToString() << std::endl;
}
```
Output:
``` 
{
  "array" : [true, "Two", 3, 4.000000],
  "array2" : [false, "three"],
  "new" : {
    "some" : {
      "deep" : {
        "key" : "Value"
      }
    }
  },
  "obj" : {
    "inner" : "Inside"
  }
}
```  
