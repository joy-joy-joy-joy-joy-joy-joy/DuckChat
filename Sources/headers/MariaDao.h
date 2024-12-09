//
// Created by w2552 on 2024/12/8.
//

#ifndef GATESERVER_MARIADAO_H
#define GATESERVER_MARIADAO_H

#include "MariaPool.h"
#include <iostream>

class MariaDao
{
public:
    MariaDao();
    ~MariaDao();
    int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
private:
    std::unique_ptr<MariaPool> pool_;
};


#endif //GATESERVER_MARIADAO_H
