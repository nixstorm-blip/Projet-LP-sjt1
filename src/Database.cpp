#include "Database.h"

Database::Database() : db(nullptr) {}

Database::~Database() { disconnect(); }

bool Database::connect(const std::string& filename) {
    int rc = sqlite3_open(filename.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Erreur : " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    std::cout << "Connexion SQLite OK : " << filename << std::endl;
    return true;
}

void Database::disconnect() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Database::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Erreur SQL : " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

std::vector<std::vector<std::string>> Database::fetchAll(const std::string& query) {
    std::vector<std::vector<std::string>> results;
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Erreur prepare : " << sqlite3_errmsg(db) << std::endl;
        return results;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::vector<std::string> row;
        int cols = sqlite3_column_count(stmt);
        for (int i = 0; i < cols; i++) {
            const char* val = (const char*)sqlite3_column_text(stmt, i);
            row.push_back(val ? val : "");
        }
        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return results;
}
