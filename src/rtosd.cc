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
            std::string return_bytes;

            try{
                if( request.ref().size() != 32 ){

                    Ref ref(request.ref().c_str(), 32);

                    if(request.has_append()){
                        if(request.append().offset() > 0){
                            response.set_result(rtos::Response::MALFORMED_REQUEST);
                        }
                        else{
                            backend->append(ref, request.append().data());
                            response.set_result(rtos::Response::SUCCESS);
                        }
                    }
                    else if(request.has_store()){
                        backend->store(ref, request.store().data());
                        response.set_result(rtos::Response::SUCCESS);
                    }
                    else if(request.has_fetch()){
                        if(request.fetch().head_num_bytes() > 0){
                            return_bytes = backend->fetch_head(ref, request.fetch().head_num_bytes()).data();
                        }
                        else if(request.fetch().tail_num_bytes() > 0){
                            return_bytes = backend->fetch_tail(ref, request.fetch().tail_num_bytes()).data();
                        }
                        else if(request.fetch().num_bytes() > 0){
                            return_bytes = backend->fetch(ref, request.fetch().from(), request.fetch().num_bytes()).data();
                        }
                        else{
                            return_bytes = backend->fetch(ref).data();
                        }

                        if(return_bytes.size() > 0){
                            response.set_result(rtos::Response::BYTES_TO_FOLLOW);
                        }
                        else{
                            response.set_result(rtos::Response::SUCCESS);
                        }
                    }
                    else{
                        response.set_result(rtos::Response::MALFORMED_REQUEST);
                    }
                }
                else{
                    response.set_result(rtos::Response::MALFORMED_REQUEST);
                }
            }
            catch(E_OBJECT_DNE e){
                response.set_result(rtos::Response::E_OBJECT_DNE);
            }
            catch(E_OBJECT_EXISTS e){
                response.set_result(rtos::Response::E_OBJECT_EXISTS);
            }

            std::string serialized_response;
            response.SerializeToString(&serialized_response);
            client->send(serialized_response);
            if(response.result() == rtos::Response::BYTES_TO_FOLLOW){
                assert(return_bytes.size() > 0);
                client->send(return_bytes);
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

    std::string DIRECTORY;
    std::string UNIX_DOMAIN_SOCKET;
    size_t WIDTH;
    size_t DEPTH;

	po::options_description desc("Options");
	desc.add_options()
        ("directory", po::value<std::string>(&DIRECTORY), "Backend storage directory")
        ("uds", po::value<std::string>(&UNIX_DOMAIN_SOCKET), "Unix Domain Socket to listen on")
        ("width", po::value<size_t>(&WIDTH), "Width of backend directory tree at each level * 16 (hex characters)")
        ("depth", po::value<size_t>(&DEPTH), "Depth of backend directory tree")
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
        (UNIX_DOMAIN_SOCKET.size() == 0) ||
        (WIDTH == 0) ||
        (DEPTH == 0)
      ){
        std::cout << desc << std::endl;
        return -1;
    }

    std::shared_ptr<Object_Store> backend(new FS_Store(DIRECTORY, DEPTH, WIDTH));

    std::shared_ptr<smpl::Local_Address> unix_domain_socket(new smpl::Local_UDS(UNIX_DOMAIN_SOCKET));
    auto unix_domain_handler = std::thread(std::bind(handle_local_address, unix_domain_socket));
    unix_domain_handler.detach();

	while(true){
		std::this_thread::sleep_for(std::chrono::seconds(3600));
	}

    return 0;
}
