#ifndef __OBJECT_STORE_H__
#define __OBJECT_STORE_H__

#include "types.h"

class E_OBJECT_DNE {}; //anything where an id lookup fails

class E_OBJECT_EXISTS {}; //mutate, if target exists

class E_DATA_EXISTS {}; //append, if data already exists

class E_DATA_DNE {}; //read, if data doesn't exist (i.e. bounds check fail)

class E_UNKNOWN {}; //unknown error, used for unsupported operation

struct Stats {
    size_t size;
};

class Object_Store {

    public:

		/*
        void create(const R_Ref &read_id, const W_Ref &write_id, const D_Ref &rm_id);
        void remove(const D_Ref &rm_id);

        void append(const W_Ref &write_id, const R_Ref &read_id, const uint64_t &index, const Object &data);
        void append(const W_Ref &write_id, const Object &data);
        void mutate(const R_Ref &target_read_id, const W_Ref &target_write_id, const D_Ref &target_rm_id, const R_Ref &source_read_id, const int64_t &index, const Object &data);

        Object read(const R_Ref &read_id, const int64_t &index, const size_t &num_bytes) const;
        Stats stat(const R_Ref &read_id);
        */

};
#endif
