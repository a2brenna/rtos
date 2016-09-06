#ifndef __OBJECT_STORE_H__
#define __OBJECT_STORE_H__

#include "types.h"

class Object_Store {

    public:

        virtual void write(const Id &id, const Data &data) = 0;
        virtual Data read(const Id &id) const = 0;

};
#endif
