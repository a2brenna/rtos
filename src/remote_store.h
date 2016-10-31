#ifndef __REMOTE_STORE_H__
#define __REMOTE_STORE_H__

#include "object_store.h"
#include "wire_protocol.pb.h"
#include <memory>
#include <smpl.h>

class Remote_Store : public Object_Store{

    public:

		void store(const Ref&id, const Object &data);
        void append(const Ref&id, const Object &data);
        void append(const Ref&id, const char *data, const size_t &size);
        Object fetch(const Ref&id) const;
        Object fetch_from(const Ref&id, const size_t &start) const;
        Object fetch(const Ref&id, const size_t &start, const size_t &num_bytes) const;
        Object fetch_head(const Ref&id, const size_t &num_bytes) const;
        Object fetch_tail(const Ref&id, const size_t &num_bytes) const;
        void fetch(const Ref&id, const size_t &start, const size_t &num_bytes, char *buf) const;
        void fetch_head(const Ref&id, const size_t &num_bytes, char *buf) const;
        void fetch_tail(const Ref&id, const size_t &num_bytes, char *buf) const;

        Remote_Store(std::shared_ptr<smpl::Remote_Address> server_address);

    private:

        std::shared_ptr<smpl::Remote_Address> _server_address;
        mutable std::shared_ptr<smpl::Channel> _server;

        rtos::Response _perform(const rtos::Request &request) const;

};

#endif
