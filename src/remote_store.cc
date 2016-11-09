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

rtos::Response _perform(const rtos::Request &request, std::shared_ptr<smpl::Channel> _server){
    std::string serialized_request;
    request.SerializeToString(&serialized_request);

    _server->send(serialized_request);
    const std::string serialized_response = _server->recv();

    rtos::Response response;
    response.ParseFromString(serialized_response);

    if(response.result() == rtos::Response::MALFORMED_REQUEST){
        assert(false);
    }
    else if(response.result() == rtos::Response::SUCCESS){
        assert(request.has_append() || request.has_store());
    }
    else if(response.result() == rtos::Response::BYTES_TO_FOLLOW){
        assert(request.has_fetch());
    }
    else if(response.result() == rtos::Response::E_OBJECT_EXISTS){
        throw E_OBJECT_EXISTS();
    }
    else if(response.result() == rtos::Response::E_OBJECT_DNE){
        throw E_OBJECT_DNE();
    }
    else if(response.result() == rtos::Response::E_DATA_DNE){
        throw E_DATA_DNE();
    }
    else{
        assert(false);
    }

    return response;
}

void Remote_Store::store(const Ref&id, const Object &data){
    rtos::Request request;
    request.set_ref(std::string(id.buf(), 32));
    rtos::Store *store = request.mutable_store();
    store->set_data(data.data());
    const auto response = _perform(request, _server);
    return;
}

void Remote_Store::append(const Ref&id, const Object &data){
    return append(id, data.data().c_str(), data.data().size());
}

void Remote_Store::append(const Ref&id, const char *data, const size_t &size){
    rtos::Request request;
    request.set_ref(std::string(id.buf(), 32));
    rtos::Append *append = request.mutable_append();
    append->set_data(std::string(data, size));
    const auto response = _perform(request, _server);
    return;
}

Object Remote_Store::fetch(const Ref &id) const{
    return fetch_from(id, 0);
}

Object Remote_Store::fetch_from(const Ref &id, const size_t &start) const{
    rtos::Request request;
    request.set_ref(std::string(id.buf(), 32));
    rtos::Fetch *fetch = request.mutable_fetch();
    fetch->set_from(start);
    const auto response = _perform(request, _server);
    std::string obj_data = _server->recv();
    return Object(obj_data);
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
    rtos::Fetch *fetch = request.mutable_fetch();
    fetch->set_from(start);
    fetch->set_num_bytes(num_bytes);
    const auto response = _perform(request, _server);
    const size_t recved = _server->recv(buf, num_bytes);
    assert(recved == num_bytes);
    return;
}

void Remote_Store::fetch_head(const Ref&id, const size_t &num_bytes, char *buf) const{
    rtos::Request request;
    request.set_ref(std::string(id.buf(), 32));
    rtos::Fetch *fetch = request.mutable_fetch();
    fetch->set_head_num_bytes(num_bytes);
    const auto response = _perform(request, _server);
    const size_t recved = _server->recv(buf, num_bytes);
    assert(recved == num_bytes);
    return;
}

void Remote_Store::fetch_tail(const Ref&id, const size_t &num_bytes, char *buf) const{
    rtos::Request request;
    request.set_ref(std::string(id.buf(), 32));
    rtos::Fetch *fetch = request.mutable_fetch();
    fetch->set_tail_num_bytes(num_bytes);
    const auto response = _perform(request, _server);
    const size_t recved = _server->recv(buf, num_bytes);
    assert(recved == num_bytes);
    return;
}
