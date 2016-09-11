#ifndef __EPHEMERAL_STORE_H__
#define __EPHEMERAL_STORE_H__

#include "object_store.h"

#include <map>

class Ephemeral_Store : public Object_Store {

    public:
        void store(const Id &id, const Data &data);
        void append(const Id &id, const Data &data);
        Data fetch(const Id &id) const;

    private:
        std::map<Id, Data> _store;

};
#endif
