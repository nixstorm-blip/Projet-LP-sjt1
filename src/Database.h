#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>

class Database {
private:
    sqlite3 *db;

public:
    Database();
    ~Database();
    bool connect(const std::string& filename);
    void disconnect();
    bool executeQuery(const std::string& query);
    std::vector<std::vector<std::string>> fetchAll(const std::string& query);
};

#endif
