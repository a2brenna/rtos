#ifndef __EPHEMERAL_STORE_H__
#define __EPHEMERAL_STORE_H__

#include "object_store.h"

#include <map>

class Ephemeral_Store : public Object_Store {

    public:
        void write(const Id &id, const Data &data);
        Data read(const Id &id) const;

    private:
        std::map<Id, Data> _store;

};
#endif
