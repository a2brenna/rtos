#ifndef __OBJECT_STORE_H__
#define __OBJECT_STORE_H__

#include "types.h"

class E_OBJECT_EXISTS {};

class E_OBJECT_DNE {};

class Object_Store {

    public:

        virtual void store(const Ref&id, const Object &data) = 0;
        virtual void append(const Ref&id, const Object &data) = 0;
        virtual void append(const Ref&id, const char *data, const size_t &size) = 0;
        virtual Object fetch_head(const Ref&id, const size_t &num_bytes) const = 0;
        virtual Object fetch_tail(const Ref&id, const size_t &num_bytes) const = 0;
        virtual void fetch_head(const Ref&id, const size_t &num_bytes, char *buf) const = 0;
        virtual void fetch_tail(const Ref&id, const size_t &num_bytes, char *buf) const = 0;
        virtual Object fetch(const Ref&id) const = 0;
        virtual Object fetch_from(const Ref&id, const size_t &start) const = 0;
        virtual Object fetch(const Ref&id, const size_t &start, const size_t &num_bytes) const = 0;
        virtual void fetch(const Ref&id, const size_t &start, const size_t &num_bytes, char *buf) const = 0;

};
#endif
