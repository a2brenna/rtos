#include "lmdb_store.h"

#include "lmdb++.h"

LMDB_Store::LMDB_Store(const std::string &path){
    _path = path;
}

void LMDB_Store::store(const Id &id, const Data &data){
    lmdb::env _env = lmdb::env::create();
    _env.set_mapsize(1UL * 1024UL * 1024UL * 1024UL); /* 1 GiB */
    _env.open(_path.c_str(), 0, 0664);

    auto t = lmdb::txn::begin(_env);
    auto dbi = lmdb::dbi::open(t, nullptr);

    dbi.put(t, id, data);

    t.commit();
}

void LMDB_Store::append(const Id &id, const Data &data){
    auto e = fetch(id);

    e.append(data);

    store(id, e);
}

Data LMDB_Store::fetch(const Id &id) const{
    lmdb::env _env = lmdb::env::create();
    _env.set_mapsize(1UL * 1024UL * 1024UL * 1024UL); /* 1 GiB */
    _env.open(_path.c_str(), 0, 0664);

    auto t = lmdb::txn::begin(_env);
    auto dbi = lmdb::dbi::open(t, nullptr);
    auto cursor = lmdb::cursor::open(t, dbi);

    std::string value;
    std::string key = id.id();

    cursor.get(key, value, MDB_NEXT);

    cursor.close();
    t.abort();

    return Data(value);
}
