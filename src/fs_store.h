#ifndef __FS_STORE_H__
#define __FS_STORE_H__

#include "object_store.h"
#include <fstream>

class FS_Store : public Object_Store{

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

        FS_Store(const std::string &path, const size_t &depth, const size_t &width);
        FS_Store(const std::string &path, const size_t &depth, const size_t &width, const std::string &log_file);
        ~FS_Store();

    private:

        std::string _path;
        size_t _depth;
        size_t _width;

        mutable std::ofstream _log;

        std::string _find(const Ref&id) const;
        void _replay_log(std::ifstream &log_file);

};

#endif
