#ifndef __LEVELDB_STORE_H__
#define __LEVELDB_STORE_H__

#include "object_store.h"

#include <string>
#include <leveldb/db.h>

class LevelDB_Store : public Object_Store {

    public:
        void store(const Id &id, const Data &data);
        void append(const Id &id, const Data &data);
        Data fetch(const Id &id) const;

        LevelDB_Store(const std::string &path);

    private:
        std::string _path;
        leveldb::DB *_db;
        leveldb::Options _options;
        leveldb::WriteOptions _wo;

};

#endif
