#ifndef __REF_LOG_H__
#define __REF_LOG_H__

#include "types.h"
#include "object_store.h"
#include <memory>
#include <vector>
#include <chrono>

class Ref_Log {

    public:
        Ref_Log(const Id &id, std::shared_ptr<Object_Store> backend);
        std::pair<std::chrono::high_resolution_clock::time_point, Id> latest_ref() const;
        std::vector<std::pair<std::chrono::high_resolution_clock::time_point, Id>> all_refs() const;
        std::pair<std::chrono::high_resolution_clock::time_point, Data> latest_object() const;

        void append_ref(const Id &id);
        void update_object(const Data &data);

    private:
        Id _id;
        std::shared_ptr<Object_Store> _backend;

};

#endif
