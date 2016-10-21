#include <iostream>
#include <boost/program_options.hpp>
#include <cassert>
#include "object_store.h"
#include "fs_store.h"

namespace po = boost::program_options;

int main(int argc, char* argv[]){

	std::string object_name;
	std::string data;
	std::string fstore;
	bool store = false;
	bool append = false;
	bool ret = false;
    bool help = false;
    size_t depth = 5;
    size_t width = 1;

	po::options_description desc("Options");
	desc.add_options()
		("name", po::value<std::string>(&object_name), "Name of object")
		("data", po::value<std::string>(&data), "Object to store")
		("fstore", po::value<std::string>(&fstore), "FS_Store location")
		("depth", po::value<size_t>(&depth), "FS_Store depth")
		("width", po::value<size_t>(&width), "FS_Store width")
		("store", po::bool_switch(&store), "Store data")
		("append", po::bool_switch(&append), "Append data")
		("ret", po::bool_switch(&ret), "Return data")
		("help", po::bool_switch(&help), "Print help")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if( (fstore.size() == 0) || (object_name.size() == 0) ){
        std::cout << desc << std::endl;
        return -1;
    }
    else if(help){
        std::cout << desc << std::endl;
        return 0;
    }

    FS_Store fs(fstore, depth, width);

    const Ref id(object_name);

    if(store || append){
        if(data.size() == 0){
            std::string input;
            while(std::cin >> input){
                data.append(input);
            }
        }
        if(store){
            try{
                fs.store(id, data);
            }
            catch(E_OBJECT_EXISTS){
                std::cerr << "Object named: " << object_name << " already exists in Objectbase: " << fstore << std::endl;
                return -1;
            }
        }
        else if(append){
            fs.append(id, data);
        }
        else{
            assert(false);
        }
    }
    else if(ret){
        const std::string s = fs.fetch(id).data();
        std::cout << s;
    }
    else{
        std::cout << desc << std::endl;
        return -1;
    }

    return 0;
}
