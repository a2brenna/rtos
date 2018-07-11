#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "fs_store.h"
#include <cassert>
#include <memory>
#include <thread>
#include <sys/stat.h>
#include <unistd.h>
#include <smpl.h>
#include <smplsocket.h>

#include "address_parse.h"
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
                if(request.has_create()){
                    const R_Ref read_id(request.create().read_id().c_str(), RAW);
                    const W_Ref write_id(request.create().write_id().c_str(), RAW);
                    const D_Ref rm_id(request.create().rm_id().c_str(), RAW);

                    backend->create(read_id, write_id, rm_id);

                    response.set_result(rtos::Response::SUCCESS);
                }
                else if(request.has_rm()){
                    const D_Ref rm_id(request.rm().rm_id().c_str(), RAW);

                    backend->remove(rm_id);

                    response.set_result(rtos::Response::SUCCESS);
                }
                else if(request.has_append()){
                    const W_Ref write_id(request.append().write_id().c_str(), RAW);
                    const Object o(request.append().data());

                    if(request.append().read_id().size() == 32){
                        const R_Ref read_id(request.append().read_id().c_str(), RAW);
                        const uint64_t index = request.append().index();

                        backend->append(write_id, read_id, index, o);
                    }
                    else{
                        backend->append(write_id, o);
                    }

                    response.set_result(rtos::Response::SUCCESS);
                }
                else if(request.has_read()){
                    const R_Ref read_id(request.read().read_id().c_str(), RAW);
                    const int64_t index = request.read().index();
                    const uint64_t num_bytes = request.read().num_bytes();

                    const Object o = backend->read(read_id, index, num_bytes);

                    response.set_data(o.data());
                    response.set_result(rtos::Response::SUCCESS);
                }
                else{
                    response.set_result(rtos::Response::E_UNKNOWN);
                }
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
            catch(E_DATA_EXISTS e){
                response.set_result(rtos::Response::E_DATA_EXISTS);
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
    std::string NETWORK_SOCKET;

	po::options_description desc("Options");
	desc.add_options()
        ("directory,d", po::value<std::string>(&DIRECTORY), "Backend storage directory")
        ("unix_domain_socket,u", po::value<std::string>(&UNIX_DOMAIN_SOCKET), "Unix Domain Socket to listen on")
        ("network_socket,n", po::value<std::string>(&NETWORK_SOCKET), "Network Socket to listen on")
    ;

    po::variables_map vm;

    try{
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

    if(vm.count("unix_domain_socket") > 0){
        struct stat statbuf;
        const int r_stat = stat(UNIX_DOMAIN_SOCKET.c_str(), &statbuf);
        if(r_stat == 0){
            if(statbuf.st_size == 0){
                unlink(UNIX_DOMAIN_SOCKET.c_str());
            }
        }

        std::shared_ptr<smpl::Local_Address> unix_domain_socket(new smpl::Local_UDS(UNIX_DOMAIN_SOCKET));
        auto unix_domain_handler = std::thread(std::bind(handle_local_address, unix_domain_socket));
        unix_domain_handler.detach();
    }

    if(vm.count("network_socket") > 0){
        const std::pair<std::string, int> address = parse_network_address(NETWORK_SOCKET);
        std::shared_ptr<smpl::Local_Address> network_socket(new smpl::Local_Port(address.first, address.second));
        auto network_handler = std::thread(std::bind(handle_local_address, network_socket));
        network_handler.detach();
    }

	while(true){
		std::this_thread::sleep_for(std::chrono::seconds(3600));
	}

    return 0;
}
