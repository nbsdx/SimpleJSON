
#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <utility>
#include <type_traits>

#include <iostream>

using std::map;
using std::vector;
using std::string;
using std::enable_if;
using std::is_same;
using std::is_convertible;
using std::is_integral;
using std::is_floating_point;

class JSON
{
    union BackingData {
        BackingData( double d ) : Float( d ){}
        BackingData( long   l ) : Int( l ){}
        BackingData( bool   b ) : Bool( b ){}
        BackingData( string s ) : String( new string( s ) ){}
        BackingData()           : Int( 0 ){}

        vector<JSON>       *List;
        map<string,JSON>   *Map;
        string             *String;
        double              Float;
        long                Int;
        bool                Bool;
    } Internal;

    public:
        enum Class {
            Null,
            Object,
            Array,
            String,
            Floating,
            Integral,
            Boolean
        };

        JSON() : Type( Null ){}

        JSON( JSON&& other ) : 
            Internal( other.Internal ),
            Type( other.Type )
        { other.Type = Null; other.Internal.Map = nullptr; }

        JSON& operator=( JSON&& other ) {
            Internal = other.Internal;
            Type = other.Type;
            other.Internal.Map = nullptr;
            other.Type = Null;
            return *this;
        }

        JSON( const JSON &other ) {
            if( other.Type == Object )
                Internal.Map = 
                    new map<string,JSON>( other.Internal.Map->begin(),
                                          other.Internal.Map->end() );
            else if( other.Type == Array )
                Internal.List = 
                    new vector<JSON>( other.Internal.List->begin(),
                                      other.Internal.List->end() );
            else if( other.Type == String )
                Internal.String = 
                    new string( *other.Internal.String );
            else
                Internal = other.Internal;
            Type = other.Type;
        }

        JSON& operator=( const JSON &other ) {
            if( other.Type == Object )
                Internal.Map = 
                    new map<string,JSON>( other.Internal.Map->begin(),
                                          other.Internal.Map->end() );
            else if( other.Type == Array )
                Internal.List = 
                    new vector<JSON>( other.Internal.List->begin(),
                                      other.Internal.List->end() );
            else if( other.Type == String )
                Internal.String = 
                    new string( *other.Internal.String );
            else
                Internal = other.Internal;
            Type = other.Type;
            return *this;
        }

        ~JSON() {
            if( Type == Array )
                delete Internal.List;
            else if( Type == Object )
                delete Internal.Map;
            else if( Type == String )
                delete Internal.String;
        }

        template <typename T>
        JSON( T b, typename enable_if<is_same<T,bool>::value>::type* = 0 ) : Internal( b ), Type( Boolean ){};

        template <typename T>
        JSON( T i, typename enable_if<is_integral<T>::value && !is_same<T,bool>::value>::type* = 0 ) : Internal( (long)i ), Type( Integral ){}

        template <typename T>
        JSON( T f, typename enable_if<is_floating_point<T>::value>::type* = 0 ) : Internal( (double)f ), Type( Floating ){}

        template <typename T>
        JSON( T s, typename enable_if<is_convertible<T,string>::value>::type* = 0 ) : Internal( string( s ) ), Type( String ){}

        static JSON Make( Class type ) {
            JSON ret; ret.SetType( type );
            return ret;
        }

        template <typename T>
        void append( T arg ) {
            SetType( Array ); Internal.List->emplace_back( arg );
        }

        template <typename T, typename... U>
        void append( T arg, U... args ) {
            append( arg ); append( args... );
        }

        template <typename T>
            typename enable_if<is_same<T,bool>::value, JSON&>::type operator=( T b ) {
                SetType( Boolean ); Internal.Bool = b; return *this;
            }

        template <typename T>
            typename enable_if<is_integral<T>::value && !is_same<T,bool>::value, JSON&>::type operator=( T i ) {
                SetType( Integral ); Internal.Int = i; return *this;
            }

        template <typename T>
            typename enable_if<is_floating_point<T>::value, JSON&>::type operator=( T f ) {
                SetType( Floating ); Internal.Float = f; return *this;
            }

        template <typename T>
            typename enable_if<is_convertible<T,string>::value, JSON&>::type operator=( T s ) {
                SetType( String ); *Internal.String = string( s ); return *this;
            }

        JSON& operator[]( const string &key ) {
            SetType( Object ); return Internal.Map->operator[]( key );
        }

        JSON& operator[]( unsigned index ) {
            SetType( Array ); return Internal.List->operator[]( index );
        }

        string ToString( int depth = 1, string tab = "  ") {
            string pad = "";
            for( int i = 0; i < depth; ++i, pad += tab );

            switch( Type ) {
                case Null:
                    return "null";
                case Object: {
                    string s = "{\n";
                    bool skip = true;
                    for( auto &p : *Internal.Map ) {
                        if( !skip ) s += ",\n";
                        s += ( pad + "\"" + p.first + "\" : " + p.second.ToString( depth + 1, tab ) );
                        skip = false;
                    }
                    s += ( "\n" + pad.erase( 0, 2 ) + "}" ) ;
                    return s;
                }
                case Array: {
                    string s = "[";
                    bool skip = true;
                    for( auto &p : *Internal.List ) {
                        if( !skip ) s += ", ";
                        s += p.ToString( depth + 1, tab );
                        skip = false;
                    }
                    s += "]";
                    return s;
                }
                case String:
                    return "\"" + *Internal.String + "\"";
                case Floating:
                    return std::to_string( Internal.Float );
                case Integral:
                    return std::to_string( Internal.Int );
                case Boolean:
                    return Internal.Bool ? "true" : "false";
                default:
                    return "";
            }
            return "";
        }

    private:
        void SetType( Class type ) {
            if( type == Type )
                return;

            /// Clear out any objects that are allocated 
            /// if we're removing an object/array.
            if( Type == Object )
                delete Internal.Map;
            else if( Type == Array )
                delete Internal.List;
            else if( Type == String )
                delete Internal.String;

            /// Setup union as needed.
            switch( type ) {
            case Null:
                Internal.Map = nullptr;
                break;
            case Object:
                Internal.Map = new map<string,JSON>();
                break;
            case Array:
                Internal.List = new vector<JSON>();
                break;
            case String:
                Internal.String = new string();
                break;
            case Floating:
                Internal.Float = 0.0;
                break;
            case Integral:
                Internal.Int = 0;
                break;
            case Boolean:
                Internal.Bool = false;
                break;
            }

            Type = type;
        }

    private:

        Class Type;
};

JSON JSONArray() {
    return std::move( JSON::Make( JSON::Array ) );
}

template <typename... T>
JSON JSONArray( T... args ) {
    JSON arr = JSON::Make( JSON::Array );
    arr.append( args... );
    return std::move( arr );
}

JSON JSONObject() {
    return std::move( JSON::Make( JSON::Object ) );
}
