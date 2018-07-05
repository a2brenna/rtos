#ifndef __FS_STORE_H__
#define __FS_STORE_H__

#include "object_store.h"
#include <fstream>

class FS_Store : public Object_Store{

    public:

        void create(const R_Ref &read_id, const W_Ref &write_id, const D_Ref &rm_id);
        /*
        void remove(const D_Ref &rm_id);

        void append(const W_Ref &write_id, const R_Ref &read_id, const uint64_t &index, const Object &data);
        void append(const W_Ref &write_id, const Object &data);
        void mutate(const R_Ref &target_read_id, const W_Ref &target_write_id, const D_Ref &target_rm_id, const R_Ref &source_read_id, const int64_t &index, const Object &data);

        Object read(const R_Ref &read_id, const int64_t &index, const size_t &num_bytes) const;
        Stats stat(const R_Ref &read_id);
        */

        FS_Store(const std::string &path);
        ~FS_Store();

    private:

        std::string _path;
        std::string _find_path(const R_Ref &id) const;
        std::string _find_path(const W_Ref &id) const;
        std::string _find_path(const D_Ref &id) const;

};

#endif
