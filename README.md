# SimpleJSON
Simple C++ JSON library

## About
SimpleJSON is a lightweight JSON library for exporting data in JSON format from C++. By taking advantage of templates and operator overloading on the backend, you're able to create and work with JSON objects right away, just as you would expect from a language such as JavaScript. SimpleJSON is packaged as a single C++ Header file "json.hpp". 

## Upcoming Features
SimpleJSON is still missing some features, which I hope to get done soon!
* API Documentation. This needs to happen SOON....
* **DONE** Import JSON from a string. **json::JSON::Loads( string )**
  * Note: This needs some more testing, I feel like I messed up the strings part. Damn escape characters...
* **DONE** Convert from a JSON object to primitive. **Limited to String, Int, Float, and Bool.**
* **DONE** Allow users to assign ~~to the next available~~ **any** array index to append to the array:
```cpp
  JSON array;
  array[0] = "Value";
  array[1] = 2;
  array[2] = true;
  array[4] = 1.1; // OK: Element 3 will be initialized to null.
```
One of the biggests goals for SimpleJSON is for it to be lightweight, and small. Having complicated logic isn't bad, but it bloats the codebase in most cases. I'd like to keep things small rather than put in big features that take a ton of space.

If you run into any bugs, please submit a bug!

## Example
More examples can be found in the 'examples' directory. They're the closest thing to an API doc until I have time to write one.

```cpp
#include "json.hpp"

int main() {
  json::JSON obj;
  // Create a new Array as a field of an Object.
  obj["array"] = json::Array( true, "Two", 3, 4.0 );
  // Create a new Object as a field of another Object.
  obj["obj"] = json::Object();
  // Assign to one of the inner object's fields
  obj["obj"]["inner"] = "Inside";
  
  // We don't need to specify the type of the JSON object:
  obj["new"]["some"]["deep"]["key"] = "Value";
  obj["array2"].append( false, "three" );
  
  std::cout << obj << std::endl;
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

This example can also be written another way:
```cpp
#include "json.hpp"
#include <iostream>

using json::JSON;

int main() {
    JSON obj = {
        "array", json::Array( true, "Two", 3, 4.0 ),
        "obj", {
            "inner", "Inside"
        },
        "new", { 
            "some", { 
                "deep", { 
                    "key", "Value" 
                } 
            } 
        },
        "array2", json::Array( false, "three" )
    };

    std::cout << obj << std::endl;
```
Sadly, we don't have access to the : character in C++, so we can't use that to seperate key-value pairs, but by using commas, we can achieve a very similar effect. The other point you might notice, is that we have to explictly create arrays. This is a limitation of C++'s operator overloading rules, so we can't use the [] operator to define the array :( I'm looking into ways to make this smoother.
