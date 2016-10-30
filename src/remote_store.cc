#include "remote_store.h"
#include <cassert>
#include <chrono>
#include <boost/algorithm/string.hpp>
#include <vector>

#include "encode.h"

Remote_Store::Remote_Store(std::shared_ptr<smpl::Remote_Address> server_address){
    _server_address = server_address;
    _server = std::shared_ptr<smpl::Channel>(server_address->connect());
}

void Remote_Store::store(const Ref&id, const Object &data){
}

void Remote_Store::append(const Ref&id, const Object &data){
}

void Remote_Store::append(const Ref&id, const char *data, const size_t &size){
}

Object Remote_Store::fetch(const Ref &id) const{
    return fetch_from(id, 0);
}

Object Remote_Store::fetch_from(const Ref &id, const size_t &start) const{
}

Object Remote_Store::fetch(const Ref&id, const size_t &start, const size_t &num_bytes) const{
    std::string s;
    s.resize(num_bytes);
    fetch(id, start, num_bytes, &s[0]);
    return Object(s);
}

Object Remote_Store::fetch_head(const Ref&id, const size_t &num_bytes) const{
    std::string s;
    s.resize(num_bytes);
    fetch_head(id, num_bytes, &s[0]);
    return Object(s);
}

Object Remote_Store::fetch_tail(const Ref&id, const size_t &num_bytes) const{
    std::string s;
    s.resize(num_bytes);
    fetch_tail(id, num_bytes, &s[0]);
    return Object(s);
}

void Remote_Store::fetch(const Ref&id, const size_t &start, const size_t &num_bytes, char *buf) const{
}

void Remote_Store::fetch_head(const Ref&id, const size_t &num_bytes, char *buf) const{
}

void Remote_Store::fetch_tail(const Ref&id, const size_t &num_bytes, char *buf) const{
}
