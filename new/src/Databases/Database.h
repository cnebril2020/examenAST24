#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

class Database {
public:
    virtual bool loadFromFile(const char* filename) = 0;
    virtual bool saveToFile(const char* filename) = 0;
    virtual bool clearFile(const char* filename) = 0;
    virtual ~Database();

protected:
    static constexpr std::size_t MAX_STR = 256; // 255 characters + '\0'
    char filename[MAX_STR];
};

#endif // DATABASE_H