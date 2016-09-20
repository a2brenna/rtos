#include "fs_store.h"
#include <cassert>

#include <iostream>
#include <fstream>
#include <sys/stat.h>

FS_Store::FS_Store(const std::string &path, const size_t &depth, const size_t &width){
    _path = path;
    _depth = depth;
    _width = width;
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
    const std::string p = _find(id);
    struct stat s;
    const auto r = stat( p.c_str(), &s);
    if( r == 0 ){
        throw E_EXISTS();
    }
    else{
        std::ofstream f;
        f.open(p);
        f << data.data();
        f.flush();
        f.close();
    }
}

void FS_Store::append(const Id &id, const Data &data){
    std::ofstream f;
    f.open(_find(id), std::ios::app);
    f << data.data();
    f.flush();
    f.close();
}

Data FS_Store::fetch(const Id &id) const{
    std::ifstream f;
    f.open(_find(id));
    std::string s;
    f >> s;
    f.close();
    return Data(s);
}
