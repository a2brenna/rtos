#ifndef __LMDB_STORE_H__
#define __LMDB_STORE_H__

#include "object_store.h"
#include "lmdb++.h"

class LMDB_Store: public Object_Store {

    public:
        LMDB_Store(const std::string &path);

        void store(const Id &id, const Data &data);
        void append(const Id &id, const Data &data);
        Data fetch(const Id &id) const;

    private:

        std::string _path;

};
#endif
