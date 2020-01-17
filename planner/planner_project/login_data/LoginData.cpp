#include <iostream>
#include "LoginData.h"

void LoginData::open_file()
{
    file.open("login_data.txt");
}

void LoginData::close_file()
{
    file.close();
}

std::string LoginData::read_login()
{
    std::string login;
    getline(file, login);
    return login;
}

std::string LoginData::read_password()
{
    std::string password;
    getline(file, password);
    return password;
}
