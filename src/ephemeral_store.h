#ifndef __EPHEMERAL_STORE_H__
#define __EPHEMERAL_STORE_H__

#include "object_store.h"

#include <map>

class Ephemeral_Store : public Object_Store {

    public:
        void store(const Ref&id, const Object &data);
        void append(const Ref&id, const Object &data);
        Object fetch(const Ref&id) const;

    private:
        std::map<Ref, Object> _store;

};
#endif
