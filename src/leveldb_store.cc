#include "leveldb_store.h"

#include <cassert>
#include <leveldb/db.h>

void LevelDB_Store::store(const Id &id, const Data &data){
    _db->Put(_wo, id.base16(), data.data());
}

void LevelDB_Store::append(const Id &id, const Data &data){
    auto r = fetch(id);
    r.append(data);
    store(id, r);
}

Data LevelDB_Store::fetch(const Id &id) const{
    std::string r;
    _db->Get(leveldb::ReadOptions(), id.base16(), &r);
    return Data(r);
}

LevelDB_Store::LevelDB_Store(const std::string &path){
    _path = path;
    _options.create_if_missing = true;
    _wo = leveldb::WriteOptions();
    _wo.sync = true;

    leveldb::Status s = leveldb::DB::Open(_options, _path, &_db);
    assert(s.ok());
}
