#include "json.hpp"

namespace json {
JSON JSON::Load( const string &str ) {
    size_t offset = 0;
    return std::move( parse_next( str, offset ) );
}

JSON Object() {
    return std::move( JSON::Make( JSON::Class::Object ) );
}

std::ostream& operator<<( std::ostream &os, const JSON &json ) {
    os << json.dump();
    return os;
}

} // namespace json
