#include "remote_store.h"
#include <cassert>
#include <chrono>
#include <boost/algorithm/string.hpp>
#include <vector>

#include "wire_protocol.pb.h"
#include "encode.h"

Remote_Store::Remote_Store(std::shared_ptr<smpl::Remote_Address> server_address){
    _server_address = server_address;
    _server = std::shared_ptr<smpl::Channel>(server_address->connect());
}

rtos::Response Remote_Store::_perform(const rtos::Request &request) const{
    std::string serialized_request;
    request.SerializeToString(&serialized_request);

    _server->send(serialized_request);
    const std::string serialized_response = _server->recv();

    rtos::Response response;
    response.ParseFromString(serialized_response);
    return response;
}

void Remote_Store::store(const Ref&id, const Object &data){
    rtos::Request request;
    request.set_ref(std::string(id.buf(), 32));
    rtos::Append *append = request.mutable_append();
    append->set_data(data.data());
    append->set_offset(0);
    const auto response = _perform(request);
}

void Remote_Store::append(const Ref&id, const Object &data){
    return append(id, data.data().c_str(), data.data().size());
}

void Remote_Store::append(const Ref&id, const char *data, const size_t &size){
    rtos::Request request;
    request.set_ref(std::string(id.buf(), 32));
    rtos::Append *append = request.mutable_append();
    append->set_data(std::string(data, size));
    const auto response = _perform(request);
}

Object Remote_Store::fetch(const Ref &id) const{
    return fetch_from(id, 0);
}

Object Remote_Store::fetch_from(const Ref &id, const size_t &start) const{
    rtos::Request request;
    request.set_ref(std::string(id.buf(), 32));
    const auto response = _perform(request);
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
    rtos::Request request;
    request.set_ref(std::string(id.buf(), 32));
    const auto response = _perform(request);
}

void Remote_Store::fetch_head(const Ref&id, const size_t &num_bytes, char *buf) const{
    rtos::Request request;
    request.set_ref(std::string(id.buf(), 32));
    const auto response = _perform(request);
}

void Remote_Store::fetch_tail(const Ref&id, const size_t &num_bytes, char *buf) const{
    rtos::Request request;
    request.set_ref(std::string(id.buf(), 32));
    const auto response = _perform(request);
}
