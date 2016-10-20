#ifndef __OBJECT_STORE_H__
#define __OBJECT_STORE_H__

#include "types.h"

class E_OBJECT_EXISTS {};

class E_OBJECT_DNE {};

class Object_Store {

    public:

        virtual void store(const Id &id, const Data &data) = 0;
        virtual void append(const Id &id, const Data &data) = 0;
        virtual void append(const Id &id, const char *data, const size_t &size) = 0;
        virtual Data fetch_head(const Id &id, const size_t &num_bytes) const = 0;
        virtual Data fetch_tail(const Id &id, const size_t &num_bytes) const = 0;
        virtual Data fetch(const Id &id) const = 0;
        virtual Data fetch(const Id &id, const size_t &start, const size_t &num_bytes) const = 0;

};
#endif
