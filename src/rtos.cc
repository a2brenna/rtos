#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <cassert>
#include <smplsocket.h>

#include "wire_protocol.pb.h"
#include "remote_store.h"
#include "encode.h"

namespace po = boost::program_options;

int main(int argc, char* argv[]){

    std::string read_ref;
    std::string write_ref;
    std::string delete_ref;
    std::string source_ref;

    bool create = false;
    bool del = false;
    bool append = false;
    bool mutate = false;
    bool read = false;
    bool stat = false;

    bool hash_ids = false;

    std::string bytes;
    int64_t index;
    size_t num_bytes;

    std::string address;

	po::options_description desc("Options");
	desc.add_options()
        ("read_ref,r", po::value<std::string>(&read_ref), "Read reference")
        ("write_ref,w", po::value<std::string>(&write_ref), "Write reference")
        ("delete_ref,d", po::value<std::string>(&delete_ref), "Delete reference")
        ("source_ref,s", po::value<std::string>(&source_ref), "Source reference")
        ("bytes,b", po::value<std::string>(&bytes), "Bytes to append/mutate")
        ("index,i", po::value<int64_t>(&index), "Index within target object")
        ("num_bytes,n", po::value<size_t>(&num_bytes), "Number of bytes to retrieve")
        ("hash_ids,h", po::bool_switch(&hash_ids), "SHA256 hash ids to get 256 bit references/keys")
        ("create,C", po::bool_switch(&create), "Create an object")
        ("delete,D", po::bool_switch(&del), "Delete an object")
        ("append,A", po::bool_switch(&append), "Append bytes to an object")
        ("mutate,M", po::bool_switch(&mutate), "Create a new object by mutating the bytes of an existing object")
        ("read,R", po::bool_switch(&read), "Read bytes from an object")
        ("stat,S", po::bool_switch(&stat), "Retrieve object meta-data")
        ("address,a", po::value<std::string>(&address), "Address of object store [hostname/ip:port | unix domain socket]")
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

    const std::function<Remote_Store()> server = [address](){
        return Remote_Store(std::shared_ptr<smpl::Remote_Address>(new smpl::Remote_UDS(address)));
    };

    if(create && !(del || append || mutate || read || stat)){
        try{
            //server().create(read_ref, write_ref, delete_ref);
        }
        catch(E_OBJECT_EXISTS e){
            std::cout << "Error: Object Exists" << std::endl;
            return -2;
        }
    }
    else if(del && !(create || append || mutate || read || stat)){

    }
    else if(append && !(create || del || mutate || read || stat)){

    }
    else if(mutate && !(create || del || append || read || stat)){

    }
    else if(read && !(create || del || append || mutate || stat)){

    }
    else if(stat && !(create || del || append || mutate || read)){

    }
    else{
        std::cout << "Specify a single operation: -CDAMNRS" << std::endl;
        std::cout << desc << std::endl;
        return -1;
    }

    return 0;
}
