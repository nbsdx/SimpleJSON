# VERSION 2 TESTING NOTES

This Readme is super out of date.

Current status: 

Creating JSON objects from c++ isn't great - it works, but it's really verbose. There are no operator= or init lists to help things be easier.

Working with JSON objects is pretty easy if you just need to access the data in them - this version is MUCH more suited to reading from JSON than it is for creating new JSON documents. It's doable, but it takes a lot of code.

```
#include "parser.hpp"
#include "json.hpp"

using namespace nbsdx::util;

/*
echo '{"foo":[true, false], "bar":null}' | ./example
==> true

*/
int main() {
  JSONParser p(std::cin);
  JSON j = p.parse();

  std::cout << j["foo"][0].AsBoolean() << "\n";
}

```

```
// Create the above JSON
void foo() {
  JSON j = JSON::Object();
  j["foo"] = JSON::Array();
  j["foo"].EmplaceBack(JSON::Boolean(true));
  j["foo"].EmplaceBack(JSON::Boolean(false));
  j["bar"] = JSON::Null();
}
```
## TODO

Add initialization-lists for bulk-cration - 

```
JSON j({
  {
    "foo", {
      true
      false
    }
  },
  {
    "bar", nullptr
  }
});

==> { "foo" : [true, false], "bar" : null }
```


# SimpleJSON
Simple C++ JSON library

## License
Do what the fuck you want public license

## About
SimpleJSON is a lightweight JSON library for exporting data in JSON format from C++. By taking advantage of templates and operator overloading on the backend, you're able to create and work with JSON objects right away, just as you would expect from a language such as JavaScript. SimpleJSON is a single C++ Header file, "json.hpp". Feel free to download this file on its own, and include it in your project. No other requirements!

#### Platforms
SimpleJSON should work on any platform; it's only requirement is a C++11 compatible compiler, as it make heavy use of the C++11 move semantics, and variadic templates. The tests are tailored for linux, but could be ported to any platform with python support and a C++11 compiler.

## API
You can find the API [over here](API.md). For now it's just a Markdown file with C++ syntax highlighting, but it's better than nothing!

## Upcoming Features
SimpleJSON is still missing some features, which I hope to get done soon!
* Write more test cases to cover all major components( mostly parsing )

One of the biggests goals for SimpleJSON is for it to be lightweight, and small. Having complicated logic isn't bad, but it bloats the codebase in most cases. I'd like to keep things small rather than put in big features that take a ton of space.

If you run into any bugs, or see that I'm missing a featuer, please submit an issue through GitHub and I'll respond as soon as I can!

## Example
More examples can be found in the 'examples' directory. Check out [the API](API.md) for a full list of functions.

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
  
  // We can also parse a string into a JSON object:
  obj["parsed"] = JSON::Load( "[ { \"Key\" : \"Value\" }, false ]" );
  
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
  },
  "parsed" : [{
      "Key" : "Value"
    }, false]
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
