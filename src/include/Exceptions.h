//
// Created by baidu on 2019/3/11.
//

#ifndef STELLAR_EXCEPTIONS_H
#define STELLAR_EXCEPTIONS_H

class command_error : public std::exception {
public:
    command_error() {}
    explicit command_error(std::string &&info) : info(info) {}
    const char* what() {
        return info.c_str();
    }
private:
    std::string info;
};

class name_not_defined : public command_error {
public:
    explicit name_not_defined(std::string &&type)
        : command_error(type + " name not defined") {}
};

#endif //STELLAR_EXCEPTIONS_H
