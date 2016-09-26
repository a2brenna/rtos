#include "fs_store.h"
#include <cassert>
#include <chrono>

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

FS_Store::~FS_Store(){
    _log.close();
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
