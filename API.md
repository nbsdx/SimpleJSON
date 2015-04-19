# SimpleJSON API

## Overview

```cpp
namespace json {

    /// Create a new JSON Array.
    JSON Array( [any_type [, ... ] ] );

    /// Create a new JSON Object.
    JSON Object();

    /// JSON Class. This is the core class.
    class JSON {

        enum Class {
            Null,
            Object,
            Array,
            String,
            Boolean,
            Floating,
            Integral
        };

        /**
            Typed Constructors

            string_type:  [const] char *, [const] char[], std::string, etc
            bool_type:    bool
            numeric_type: char, int, long, double, float, etc
            null_type:    nullptr_t

         */
        JSON( string_type );
        JSON( bool_type );
        JSON( numeric_type );
        JSON( null_type );
        
        /** 
            Copy/Move Constructors
         */
        JSON( const JSON & );
        JSON( JSON && );

        /**
            Static Methods
         */

        /// Create a JSON object from a string.
        JSON Load( string_type );

        /// Create a JSON object with the specified json::Class type.
        JSON Make( JSON::Class );

        /**
            Operator Overloading

            Assigning to a JSON object changes the type on the fly.
            If you have a JSON object that represents an integer, 
            and then you assign a boolean value to it, that object
            now represents a boolean.
         */

        /// Assign a boolean type to a JSON object
        JSON& operator=( bool_type );

        /// Assign a numeric type to a JSON object
        JSON& operator=( numeric_type );

        /// Assign a string type to a JSON object
        JSON& operator=( string_type );

        /// Assign a null type to a JSON object
        // JSON& operator=( null_type ); // TODO: Not Impld

        /// Standard copy/move assignment operators
        JSON& operator=( const JSON & );
        JSON& operator=( JSON && );

        /// Access the elements of a JSON Object.
        /// Accessing an invalid key will create a new entry with a Null type.
        JSON& operator[]( string_type key );

        /// Access the elements of a JSON Array. 
        /// Accessing an out of bounds index will extend the Array.
        JSON& operator[]( unsigned index );

        /// Stream operator; calls dump()
        std::ostream& operator<<( std::ostream &, const JSON & );

        /**
            Utility Methods
         */

        /// Useful for appending to an Array, can take any number of
        /// primitive types using variadic templates
        void append( any_type [, ... ] );

        /// Dumps the JSON object to a string format for storing.
        void dump( int depth = 0, string indent = "  " );

        /// Get the JSON::Class type for a JSON object.
        JSON::Class JSONType();

        /// Convience method to determine if an object is Class::Null
        bool IsNull();

        /// Convert to a string literal iff Type == Class::String
        string ToString();
        string ToString( bool &OK );

        /// Convert to a floating literal iff Type == Class::Floating
        double ToFloat();
        double ToFloat( bool &OK );

        /// Convert to an integral literal iff Type == Class::Integral
        long ToInt();
        long ToInt( bool &OK );

        /// Convert to a boolean literal iff Type == Class::Boolean
        bool ToBool();
        bool ToBool( bool &OK );

        /**
            Iterating
        */

        /// Wraps the internal object representation to access iterators.
        /// Will return empty range for non-object objects.
        JSONWrapper ObjectRange();

        /// Wraps the internal array representation to access iterators.
        /// Will return empty range for non-array objects.
        JSONWrapper ArrayRange();
    }; // End json::JSON documentation
} // End json documentation

