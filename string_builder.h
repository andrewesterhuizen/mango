#pragma once

#include <string>

namespace string_builder {

class StringBuilder {
    std::string string;
    int indent = 0;
    int indent_spaces = 2;

public:
    void increase_indent() { indent += indent_spaces; }
    void decrease_indent() { indent -= indent_spaces; }
    void append(std::string s);
    void append_no_indent(std::string s);
    void append_line(std::string s);
    void append_line_no_indent(std::string s);
    std::string get_string();
};

}