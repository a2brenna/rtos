#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "fs_store.h"
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

            rtos::Response response;
            std::string return_bytes;

            try{

            }
            catch(E_OBJECT_DNE e){
                response.set_result(rtos::Response::E_OBJECT_DNE);
            }
            catch(E_OBJECT_EXISTS e){
                response.set_result(rtos::Response::E_OBJECT_EXISTS);
            }
            catch(E_DATA_DNE e){
                response.set_result(rtos::Response::E_DATA_DNE);
            }

            std::string serialized_response;
            response.SerializeToString(&serialized_response);
            client->send(serialized_response);
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

    std::string DIRECTORY;
    std::string UNIX_DOMAIN_SOCKET;

	po::options_description desc("Options");
	desc.add_options()
        ("directory", po::value<std::string>(&DIRECTORY), "Backend storage directory")
        ("uds", po::value<std::string>(&UNIX_DOMAIN_SOCKET), "Unix Domain Socket to listen on")
    ;

    try{
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }
    catch(...){
        std::cout << desc << std::endl;
        return -1;
    }

    if( (DIRECTORY.size() == 0) ||
        (UNIX_DOMAIN_SOCKET.size() == 0)
      ){
        std::cout << desc << std::endl;
        return -1;
    }

    backend = std::shared_ptr<Object_Store>(new FS_Store(DIRECTORY));

    std::shared_ptr<smpl::Local_Address> unix_domain_socket(new smpl::Local_UDS(UNIX_DOMAIN_SOCKET));
    auto unix_domain_handler = std::thread(std::bind(handle_local_address, unix_domain_socket));
    unix_domain_handler.detach();

	while(true){
		std::this_thread::sleep_for(std::chrono::seconds(3600));
	}

    return 0;
}
