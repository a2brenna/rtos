#include "address_parse.h"

const std::pair<std::string, int> parse_network_address(const std::string &arg){
	size_t i = 0;
	for( ; i < arg.size() && arg[i] != ':'; i++);
    if(i == arg.size()){
        throw E_BAD_ADDRESS();
    }

	const std::string ip(arg.c_str(), i);
	const int port = atoi(arg.c_str() + i + 1);

	return std::pair<std::string, int>(ip, port);
}
