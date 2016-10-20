#include "ref_log.h"
#include <cstring>
#include <cassert>

struct Timed_Reference {
    uint64_t timestamp;
    char id[32];
};

Ref_Log::Ref_Log(const Id &id, std::shared_ptr<Object_Store> backend){
    _id = id;
    _backend = backend;
}

std::pair<std::chrono::high_resolution_clock::time_point, Id> Ref_Log::latest_ref() const{
    std::pair<std::chrono::high_resolution_clock::time_point, Id> r;

    Timed_Reference l;
    _backend->fetch_tail(_id, sizeof(Timed_Reference), (char *)&l);

    r.first = std::chrono::high_resolution_clock::time_point(std::chrono::nanoseconds(l.timestamp));
    r.second = Id(l.id, 32);

    return r;
}

std::vector<std::pair<std::chrono::high_resolution_clock::time_point, Id>> Ref_Log::all_refs() const{
    std::vector<std::pair<std::chrono::high_resolution_clock::time_point, Id>> r;

    const Data t = _backend->fetch(_id);
    assert(t.data().size() % sizeof(Timed_Reference) == 0);

    const size_t num_elements = t.data().size() / sizeof(Timed_Reference);
    const Timed_Reference *tr_array = (Timed_Reference *)t.data().c_str();

    for(size_t i = 0; i < num_elements; i++){
        r.push_back(std::pair<std::chrono::high_resolution_clock::time_point, Id>(std::chrono::high_resolution_clock::time_point(std::chrono::nanoseconds(tr_array[i].timestamp)), Id(tr_array[i].id, 32)));
    }

    return r;
}

std::pair<std::chrono::high_resolution_clock::time_point, Data> Ref_Log::latest_object() const{
    const auto l = latest_ref();
    return std::pair<std::chrono::high_resolution_clock::time_point, Data>(l.first, _backend->fetch(l.second));
}

void Ref_Log::append_ref(const Id &id){
    Timed_Reference t;
    t.timestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::memcpy(t.id, id.buf(), 32);

    _backend->append(_id, (char *)&t, sizeof(Timed_Reference));
}
void Ref_Log::update_object(const Data &object){
    const Id id;
    _backend->store(id, object);

    append_ref(id);
}
