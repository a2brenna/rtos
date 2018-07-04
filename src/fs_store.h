#ifndef __FS_STORE_H__
#define __FS_STORE_H__

#include "object_store.h"
#include <fstream>

enum REF_TYPE {
    READ,
    WRITE,
    RM
};

class FS_Store : public Object_Store{

    public:

        /*
        void create(const Ref &read_id, const Ref &write_id, const Ref &rm_id);
        void remove(const Ref &rm_id);

        void append(const Ref &write_id, const Ref &read_id, const uint64_t &index, const Object &data);
        void append(const Ref &write_id, const Object &data);
        void mutate(const Ref &target_read_id, const Ref &target_write_id, const Ref &target_rm_id, const Ref &source_read_id, const int64_t &index, const Object &data);

        Object read(const Ref &read_id, const int64_t &index, const size_t &num_bytes) const;
        Stats stat(const Ref &read_id);

        */

        FS_Store(const std::string &path);
        ~FS_Store();

    private:

        std::string _path;
        int _find_link(const enum REF_TYPE, const Ref &id) const;

};

#endif
