#ifndef __REMOTE_STORE_H__
#define __REMOTE_STORE_H__

#include "object_store.h"
#include <memory>
#include <smpl.h>
#include <mutex>

class Remote_Store : public Object_Store{

    public:

        void create(const R_Ref &read_id, const W_Ref &write_id, const D_Ref &rm_id);
        void remove(const D_Ref &rm_id);

        void append(const W_Ref &write_id, const R_Ref &read_id, const uint64_t &index, const Object &data);
        void append(const W_Ref &write_id, const Object &data);
        /*
        void mutate(const R_Ref &target_read_id, const W_Ref &target_write_id, const D_Ref &target_rm_id, const R_Ref &source_read_id, const int64_t &index, const Object &data);

        Object read(const R_Ref &read_id, const int64_t &index, const size_t &num_bytes) const;
        Stats stat(const R_Ref &read_id);
        */

        Remote_Store(std::shared_ptr<smpl::Remote_Address> server_address);

    private:

        std::shared_ptr<smpl::Remote_Address> _server_address;
        std::shared_ptr<smpl::Channel> _server;

};

#endif
