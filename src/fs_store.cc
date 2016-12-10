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

    //_log.open(_path + "/log", std::ifstream::app);
    //assert(_log);
}

//TODO: Collapse this by using some technology
FS_Store::FS_Store(const std::string &path, const size_t &depth, const size_t &width, const std::string &log_path){
    _path = path;
    _depth = depth;
    _width = width;

    //replay log
    /*
    std::ifstream l;
    l.open(log_path);
    assert(l);
    _replay_log(l);
    l.close();

    _log.open(_path + "/log", std::ifstream::app);
    assert(_log);
    */
}

FS_Store::~FS_Store(){
    /*
    _log.close();
    assert(_log);
    */
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
            const Ref key(tokens[2]);
            const Object data(base64_decode(tokens[3]));

            store(key, data);
        }
        else if(operation == "APPEND"){
            assert(tokens.size() == 4);
            const Ref key(tokens[2]);
            const Object data(base64_decode(tokens[3]));

            append(key, data);
        }
        else if(operation == "FETCH"){
            assert(false);

        }
        else{
            assert(false);
        }

    }
}

std::string FS_Store::_find(const Ref&id) const{
    std::string path = _path + "/";
    const std::string i = id.base16();

    size_t k = 0;
    for(size_t j = 0; j < _depth; j++){
        assert( (k + _width) < i.size() );
        std::string dir(i, k, _width);

        path = path + dir + "/";
        k += _width;
    }

    return path + i;
}

void FS_Store::store(const Ref&id, const Object &data){
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    _log << now << " STORE " << id.base16() << " " << base64_encode(data.data()) << std::endl;
    //assert(_log);
    const std::string p = _find(id);
    struct stat s;
    const auto r = stat( p.c_str(), &s); //TODO: investigate other return codes
    if( r == 0 ){
        throw E_OBJECT_EXISTS();
    }
    else{
        std::ofstream f;
        f.open(p);
        assert(f);
        //TODO: check for errors?
        f << data.data();
        assert(f);
        //TODO: check for errors?
        f.close();
        assert(f);
        //TODO: check for errors?
    }
}

void FS_Store::append(const Ref&id, const Object &data){
    append(id, data.data().c_str(), data.data().size());
}

void FS_Store::append(const Ref&id, const char *data, const size_t &size){
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    _log << now << " APPEND " << id.base16() << " " << base64_encode((const unsigned char*)data, size) << std::endl;
    //assert(_log);
    std::ofstream f;
    f.open(_find(id), std::ios::app);
    assert(f);
    //TODO: check for errors?
    f.write(data, size);
    assert(f);
    //TODO: check for errors?
    f.close();
    assert(f);
    //TODO: check for errors?
}

Object FS_Store::fetch(const Ref &id) const{
    return fetch_from(id, 0);
}

Object FS_Store::fetch_from(const Ref &id, const size_t &start) const{
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    _log << now << " FETCH_FROM " << id.base16() << " " << start << std::endl;

    std::ifstream f;
    f.open(_find(id), std::ios::in);

    //TODO: Be more specific in checking this error
    if(!f){
        throw E_OBJECT_DNE();
    }
    else{
        f.seekg(0, std::ios::end);
        assert(f);
        const size_t file_size = f.tellg();
        if(start > file_size){
            throw E_DATA_DNE();
        }

        std::string s;
        const size_t result_size = file_size - start;
        s.resize(result_size);

        f.seekg(start);
        f.read(&s[0], result_size);
        assert(f);

        return Object(s);
    }
}

Object FS_Store::fetch(const Ref&id, const size_t &start, const size_t &num_bytes) const{
    std::string s;
    s.resize(num_bytes);
    fetch(id, start, num_bytes, &s[0]);
    return Object(s);
}

Object FS_Store::fetch_head(const Ref&id, const size_t &num_bytes) const{
    std::string s;
    s.resize(num_bytes);
    fetch_head(id, num_bytes, &s[0]);
    return Object(s);
}

Object FS_Store::fetch_tail(const Ref&id, const size_t &num_bytes) const{
    std::string s;
    s.resize(num_bytes);
    fetch_tail(id, num_bytes, &s[0]);
    return Object(s);
}

void FS_Store::fetch(const Ref&id, const size_t &start, const size_t &num_bytes, char *buf) const{
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    _log << now << " FETCH_WINDOW " << id.base16() << " " << start << " " << num_bytes << std::endl;

    std::ifstream f;
    f.open(_find(id), std::ios::in);

    //TODO: Be more specific in checking this error
    if(!f){
        throw E_OBJECT_DNE();
    }
    else{
        f.seekg(start, std::ios::beg);
        if(!f){
            throw E_DATA_DNE();
        }

        f.read(buf, num_bytes);
        if(!f){
            throw E_DATA_DNE();
        }
    }
}

void FS_Store::fetch_head(const Ref&id, const size_t &num_bytes, char *buf) const{
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    _log << now << " FETCH_HEAD " << id.base16() << " " << num_bytes << std::endl;

    std::ifstream f;
    f.open(_find(id), std::ios::in);

    //TODO: Be more specific in checking this error
    if(!f){
        throw E_OBJECT_DNE();
    }
    else{
        f.read(buf, num_bytes);
        if(!f){
            throw E_DATA_DNE();
        }
    }
}

void FS_Store::fetch_tail(const Ref&id, const size_t &num_bytes, char *buf) const{
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    _log << now << " FETCH_TAIL " << id.base16() << " " << num_bytes << std::endl;

    std::ifstream f;
    f.open(_find(id), std::ios::in);

    //TODO: Be more specific in checking this error
    if(!f){
        throw E_OBJECT_DNE();
    }
    else{
        f.seekg(-(ssize_t)num_bytes, std::ios::end);
        if(!f){
            throw E_DATA_DNE();
        }
        f.read(buf, num_bytes);
        if(!f){
            throw E_DATA_DNE();
        }
    }
}
