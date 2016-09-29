#include "fs_store.h"
#include <cassert>
#include <chrono>
#include <boost/algorithm/string.hpp>
#include <vector>

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "encode.h"

FS_Store::FS_Store(const std::string &path, const size_t &depth, const size_t &width){
    _path = path;
    _depth = depth;
    _width = width;

    _log.open(_path + "/log");
}

//TODO: Collapse this by using some technology
FS_Store::FS_Store(const std::string &path, const size_t &depth, const size_t &width, const std::string &log_path){
    _path = path;
    _depth = depth;
    _width = width;

    //replay log
    std::ifstream l;
    l.open(log_path);
    _replay_log(l);

    _log.open(_path + "/log");
}

FS_Store::~FS_Store(){
    _log.close();
}

void FS_Store::_replay_log(std::ifstream &log_file){
    std::vector<std::string> tokens;
    std::string line;

    while(std::getline(log_file, line)){
        split(tokens, line, boost::is_any_of(" "));
        assert(tokens.size() > 1);

        const std::string operation = tokens[1];

        if(operation == "STORE"){
            assert(tokens.size() == 4);
            const Id key(tokens[2]);
            const Data data(base64_decode(tokens[3]));

            store(key, data);
        }
        else if(operation == "APPEND"){
            assert(tokens.size() == 4);
            const Id key(tokens[2]);
            const Data data(base64_decode(tokens[3]));

            append(key, data);
        }
        else if(operation == "FETCH"){

        }
        else{
            assert(false);
        }

    }
}

std::string FS_Store::_find(const Id &id) const{
    std::string path = _path + "/";
    const std::string i = id.id();

    size_t k = 0;
    for(size_t j = 0; j < _depth; j++){
        assert( (k + _width) < i.size() );
        std::string dir(i, k, _width);

        path = path + dir + "/";
        k += _width;
    }

    return path + i;
}

void FS_Store::store(const Id &id, const Data &data){
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    _log << now << " STORE " << id.id() << " " << base64_encode(data.data()) << std::endl;
    const std::string p = _find(id);
    struct stat s;
    const auto r = stat( p.c_str(), &s); //TODO: investigate other return codes
    if( r == 0 ){
        throw E_EXISTS();
    }
    else{
        std::ofstream f;
        f.open(p);
        //TODO: check for errors?
        f << data.data();
        //TODO: check for errors?
        f.close();
        //TODO: check for errors?
    }
}

void FS_Store::append(const Id &id, const Data &data){
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    _log << now << " APPEND " << id.id() << " " << base64_encode(data.data()) << std::endl;
    std::ofstream f;
    f.open(_find(id), std::ios::app);
    //TODO: check for errors?
    f << data.data();
    //TODO: check for errors?
    f.close();
    //TODO: check for errors?
}

Data FS_Store::fetch(const Id &id) const{
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    _log << now << " FETCH " << id.id() << std::endl;
    std::ifstream f;
    f.open(_find(id));
    //TODO: check for errors?
    std::string s;
    f >> s;
    //TODO: check for errors?
    f.close();
    //TODO: check for errors?
    return Data(s);
}