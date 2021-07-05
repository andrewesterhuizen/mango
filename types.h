#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace mango {

enum class DataType {
    Undefined,
    Integer,
//    Float,
    String,
    Bool,
    Function,
    Object,
    Array
};


std::string data_type_to_string(const DataType dt);

std::ostream &operator<<(std::ostream &os, const DataType &dt);

struct Object {
    std::unordered_map<std::string, Object*> properties;

    virtual std::string to_string() {
        std::string out = "{\n";

        for (auto prop : properties) {
            out += "  " + prop.first + ": " + prop.second->to_string() + ",\n";
        }

        out += "}";
        return out;
    }

    virtual DataType type() { return DataType::Object; }
    virtual bool is_truthy() { return true; }
};


struct Undefined : public Object {
    virtual bool is_truthy() override { return false; }
    virtual DataType type() override { return DataType::Undefined; }
    std::string to_string() override { return "undefined"; }
};

struct Statement;

struct Function : public Object {
    std::vector<std::string> parameters;
    Statement* body;
    bool is_builtin;
    std::function<Object*(std::vector<Object*>)> builtin_fn;
    virtual DataType type() override { return DataType::Function; }
    std::string to_string() override { return "function"; }
};

struct Array : public Object {
    std::vector<Object*> elements;
    virtual DataType type() override { return DataType::Array; }
    std::string to_string() override {
        std::string out = "[";
        for (auto el : elements) {
            out += el->to_string() + ", ";
        }
        out += "]";
        return out;
    }
};

struct Integer : public Object {
    int value = 0;
    Integer(int v) : value(v) {}
    virtual bool is_truthy() override { return value != 0; }
    virtual DataType type() override { return DataType::Integer; }
    std::string to_string() override { return std::to_string(value); }
};

struct String : public Object {
    std::string value;
    String(std::string v) : value(v) {}
    virtual DataType type() override { return DataType::String; }
    std::string to_string() override { return "\"" + value + "\""; }
};

struct Bool : public Object {
    bool value;
    Bool(bool v) : value(v) {}
    virtual bool is_truthy() override { return value; }
    virtual DataType type() override { return DataType::Bool; }
    virtual std::string to_string() override { return value ? "true" : "false"; }
};

std::ostream &operator<<(std::ostream &os, Object &o);

}