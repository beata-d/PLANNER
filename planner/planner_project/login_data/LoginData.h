#ifndef PLANNER_LOGINDATA_H
#define PLANNER_LOGINDATA_H

#include <cstdio>
#include <string>
#include <array>
#include <fstream>

class LoginData {
public:
    LoginData() = default;
    virtual ~LoginData() = default;
    void open_file();
    void close_file();
    std::string read_login();
    std::string read_password();
private:
    std::ifstream file;
};

#endif //PLANNER_LOGINDATA_H
