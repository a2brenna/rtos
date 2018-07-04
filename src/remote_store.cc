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

rtos::Response perform_request(const rtos::Request &request, std::shared_ptr<smpl::Channel> _server){
    std::string serialized_request;
    request.SerializeToString(&serialized_request);

    _server->send(serialized_request);
    const std::string serialized_response = _server->recv();

    rtos::Response response;
    response.ParseFromString(serialized_response);

    if(response.result() == rtos::Response::E_OBJECT_EXISTS){
        throw E_OBJECT_EXISTS();
    }
    else if(response.result() == rtos::Response::E_OBJECT_DNE){
        throw E_OBJECT_DNE();
    }
    else if(response.result() == rtos::Response::E_DATA_EXISTS){
        throw E_DATA_EXISTS();
    }
    else if(response.result() == rtos::Response::E_DATA_DNE){
        throw E_DATA_DNE();
    }
    else{
        return response;
    }

}

void Remote_Store::create(const Ref &read_id, const Ref &write_id, const Ref &rm_id){
    rtos::Request request;

    rtos::Create *v = request.mutable_create();
    v->set_read_id(read_id.buf(), 32);
    v->set_write_id(write_id.buf(), 32);
    v->set_rm_id(rm_id.buf(), 32);

    perform_request(request, _server);
}
