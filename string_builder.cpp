#include "string_builder.h"

namespace string_builder {

void StringBuilder::append(std::string s) {
    for (int i = 0; i < indent; ++i) {
        string += " ";
    }

    string += s;
}

void StringBuilder::append_no_indent(std::string s) {
    string += s;
}

void StringBuilder::append_line(std::string s) {
    for (int i = 0; i < indent; ++i) {
        string += " ";
    }

    string += s + "\n";
}

void StringBuilder::append_line_no_indent(std::string s) {
    string += s + "\n";
}

std::string StringBuilder::get_string() {
    return string;
}

}