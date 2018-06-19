#ifndef __OBJECT_STORE_H__
#define __OBJECT_STORE_H__

#include "types.h"

class E_OBJECT_EXISTS {};

class E_OBJECT_DNE {};

class E_DATA_DNE {};

struct Stats {
    size_t size;
};

class Object_Store {

    public:

        virtual void create(const Ref &read_id, const Ref &write_id, const Ref &rm_id) = 0;
        virtual void remove(const Ref &rm_id) = 0;

        virtual void append(const Ref &write_id, const Ref &read_id, const uint64_t &index, const Object &data) = 0;
        virtual void append(const Ref &write_id, const Object &data) = 0;
        virtual void mutate(const Ref &target_read_id, const Ref &target_write_id, const Ref &target_rm_id, const Ref &source_read_id, const int64_t &index, const Object &data) = 0;

        virtual Object read(const Ref &read_id, const int64_t &index, const size_t &num_bytes) const = 0;
        virtual Stats stat(const Ref &read_id) = 0;

};
#endif
