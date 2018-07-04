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

/*
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
    else if(response.result() == rtos::Response::SUCCESS){
    }
    else{
        assert(false);
    }

    return response;
}
*/
