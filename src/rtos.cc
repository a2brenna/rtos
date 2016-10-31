#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <cassert>
#include <memory>
#include <thread>
#include <smplsocket.h>

#include "object_store.h"
#include "remote_store.h"

namespace po = boost::program_options;

std::string UNIX_DOMAIN_SOCKET;

int main(int argc, char* argv[]){

    std::string name;
    std::string ref_16;
    std::string data;

	po::options_description desc("Options");
	desc.add_options()
        ("uds", po::value<std::string>(&UNIX_DOMAIN_SOCKET), "Server Unix Domain Socket")
        ("name", po::value<std::string>(&name), "Object name to be hashed to reference")
        ("ref16", po::value<std::string>(&ref_16), "Base16 encoded 256 bit reference")
        ("append", po::value<std::string>(&data), "Data to append")
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

    std::shared_ptr<Object_Store> server(new Remote_Store(std::shared_ptr<smpl::Remote_Address>(new smpl::Remote_UDS(UNIX_DOMAIN_SOCKET))));

    return 0;
}
