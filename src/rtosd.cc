#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <rtos/fs_store.h>
#include <cassert>
#include <memory>
#include <thread>
#include <smpl.h>
#include <smplsocket.h>

#include "wire_protocol.pb.h"

namespace po = boost::program_options;

std::shared_ptr<Object_Store> backend;

void handle_channel(std::shared_ptr<smpl::Channel> client){
    for(;;){
        try{
            const std::string serialized_request = client->recv();
            rtos::Request request;
            request.ParseFromString(serialized_request);
            std::cout << request.DebugString() << std::endl;

            rtos::Response response;
            std::string object_bytes;

            std::string serialized_response;
            response.SerializeToString(&serialized_response);
            client->send(serialized_response);
            if(response.result() == rtos::Response::BYTES_TO_FOLLOW){
                assert(object_bytes.size() > 0);
                client->send(object_bytes);
            }
        }
        catch(smpl::Transport_Failed t){
            break;
        }
    }
}

void handle_local_address(std::shared_ptr<smpl::Local_Address> incoming){
  for(;;){
    std::shared_ptr<smpl::Channel> new_client(incoming->listen());
    auto t = std::thread(std::bind(handle_channel, new_client));
    t.detach();
  }
}

int main(int argc, char* argv[]){

    std::string DIRECTORY = "./fs_store.fs";
    std::string UNIX_DOMAIN_SOCKET = "./rtos.sock";
    size_t WIDTH = 1;
    size_t DEPTH = 5;

	po::options_description desc("Options");
	desc.add_options()
        ("directory", po::value<std::string>(&DIRECTORY), "Backend storage directory")
        ("uds", po::value<std::string>(&UNIX_DOMAIN_SOCKET), "Unix Domain Socket to listen on")
        ("width", po::value<size_t>(&WIDTH), "Width of backend directory tree at each level * 16 (hex characters)")
        ("depth", po::value<size_t>(&DEPTH), "Depth of backend directory tree")
    ;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    std::shared_ptr<Object_Store> backend(new FS_Store(DIRECTORY, DEPTH, WIDTH));

    std::shared_ptr<smpl::Local_Address> unix_domain_socket(new smpl::Local_UDS(UNIX_DOMAIN_SOCKET));
    auto unix_domain_handler = std::thread(std::bind(handle_local_address, unix_domain_socket));
    unix_domain_handler.detach();

	while(true){
		std::this_thread::sleep_for(std::chrono::seconds(3600));
	}

    return 0;
}
