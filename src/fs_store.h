#ifndef __FS_STORE_H__
#define __FS_STORE_H__

#include "object_store.h"
#include <fstream>

class FS_Store : public Object_Store{

    public:

		void store(const Id &id, const Data &data);
        void append(const Id &id, const Data &data);
        Data fetch(const Id &id) const;

        FS_Store(const std::string &path, const size_t &depth, const size_t &width);
        FS_Store(const std::string &path, const size_t &depth, const size_t &width, const std::string &log_file);
        ~FS_Store();

    private:

        std::string _path;
        size_t _depth;
        size_t _width;

        mutable std::ofstream _log;

        std::string _find(const Id &id) const;
        void _replay_log(std::ifstream &log_file);

};

#endif