#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <cassert>
#include <memory>
#include <thread>
#include <smplsocket.h>

#include "object_store.h"
#include "remote_store.h"
#include "encode.h"

namespace po = boost::program_options;

std::string UNIX_DOMAIN_SOCKET;

int main(int argc, char* argv[]){

    std::string name;
    std::string ref_16;
    std::string append_data;
    std::string store_data;
    bool append_stdin = false;
    bool store_stdin = false;
    size_t tail = 0;
    size_t head = 0;
    size_t from = 0;
    size_t num_bytes = 0;

	po::options_description desc("Options");
	desc.add_options()
        ("uds", po::value<std::string>(&UNIX_DOMAIN_SOCKET), "Server Unix Domain Socket")
        ("name", po::value<std::string>(&name), "Object name to be hashed to reference")
        ("ref16", po::value<std::string>(&ref_16), "Base16 encoded 256 bit reference")
        ("append", po::value<std::string>(&append_data), "Data to append")
        ("store", po::value<std::string>(&store_data), "Data to store")
        ("append-stdin", po::bool_switch(&append_stdin), "Append from stdin")
        ("store-stdin", po::bool_switch(&store_stdin), "Store from stdin")
        ("tail", po::value<size_t>(&tail), "Number of bytes to fetch from tail")
        ("head", po::value<size_t>(&head), "Number of bytes to fetch from head")
        ("from", po::value<size_t>(&from), "Offset at which to begin fetch")
        ("num_bytes", po::value<size_t>(&num_bytes), "Number of bytes to fetch")
    ;

	po::variables_map vm;
    try{
        po::store(po::parse_command_line(argc, argv, desc), vm);
    }
    catch(...){
        std::cout << "Error parsing arguments" << std::endl;
        std::cout << desc << std::endl;
        return -1;
    }
    po::notify(vm);

    if( (UNIX_DOMAIN_SOCKET.size() == 0) ||
        ((name.size() == 0) && (ref_16.size() == 0))
      ){
        std::cout << desc << std::endl;
        return -1;
    }

    Ref id;
    if(name.size() > 0){
        id = Ref(name);
    }
    else if(ref_16.size() > 0){
        const std::string raw_ref = base16_decode(ref_16);
        if(raw_ref.size() == 32){
            id = Ref(raw_ref.c_str(), 32);
        }
        else{
            std::cout << desc << std::endl;
            return -1;
        }
    }

    std::shared_ptr<Object_Store> server(new Remote_Store(std::shared_ptr<smpl::Remote_Address>(new smpl::Remote_UDS(UNIX_DOMAIN_SOCKET))));

    try{
        if(append_data.size() > 0){
            server->append(id, append_data);
        }
        else if(store_data.size() > 0){
            server->store(id, store_data);
        }
        else if(append_stdin){
            std::string input;
            while(std::cin >> input){
                server->append(id,input);
            }
        }
        else if(store_stdin){
            std::string input;
            std::cin >> input;
            server->store(id, input);
            while(std::cin >> input){
                server->append(id,input);
            }
        }
        else if(tail > 0){
            std::cout << server->fetch_tail(id, tail).data();
        }
        else if(head > 0){
            std::cout << server->fetch_head(id, head).data();
        }
        else if( num_bytes > 0 ){
            std::cout << server->fetch(id, from, num_bytes).data();
        }
        else{
            std::cout << server->fetch(id).data();
        }
    }
    catch(E_OBJECT_EXISTS e){
        std::cerr << "Object Already Exists" << std::endl;
        return 1;
    }
    catch(E_OBJECT_DNE e){
        std::cerr << "Object Does Not Exist" << std::endl;
        return 1;
    }
    catch(E_DATA_DNE e){
        std::cerr << "Data Does Not Exist" << std::endl;
        return 1;
    }
    catch(...){
        std::cerr << "Unknown Error" << std::endl;
        return 1;
    }

    return 0;
}
