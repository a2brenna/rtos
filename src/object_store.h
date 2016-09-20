#ifndef __OBJECT_STORE_H__
#define __OBJECT_STORE_H__

#include "types.h"

class E_EXISTS {

};

class Object_Store {

    public:

        virtual void store(const Id &id, const Data &data) = 0;
        virtual void append(const Id &id, const Data &data) = 0;
        virtual Data fetch(const Id &id) const = 0;

};
#endif
