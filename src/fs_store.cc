#include "fs_store.h"
#include <cassert>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "encode.h"

FS_Store::FS_Store(const std::string &path){
    _path = path;
}

FS_Store::~FS_Store(){

}

std::string FS_Store::_find_path(const R_Ref &id) const{
    return _path + "/read/" + id.base16();
}

std::string FS_Store::_find_path(const W_Ref &id) const{
    return _path + "/write/" + id.base16();
}

std::string FS_Store::_find_path(const D_Ref &id) const{
    return _path + "/delete/" + id.base16();
}

void FS_Store::create(const R_Ref &read_id, const W_Ref &write_id, const D_Ref &rm_id){

    struct stat statbuf;

    const std::string r_path = _find_path(read_id);
    const int r_stat = stat(r_path.c_str(), &statbuf);
    if(r_stat != -1 || errno != ENOENT){
        throw E_OBJECT_EXISTS();
    }

    const std::string w_path = _find_path(write_id);
    const int w_stat = stat(w_path.c_str(), &statbuf);
    if(w_stat != -1 || errno != ENOENT){
        throw E_OBJECT_EXISTS();
    }

    const std::string d_path = _find_path(rm_id);
    const int d_stat = stat(d_path.c_str(), &statbuf);
    if(d_stat != -1 || errno != ENOENT){
        throw E_OBJECT_EXISTS();
    }

    //create_file
    const int new_fd = open(r_path.c_str(), O_CREAT | O_RDONLY, S_IRWXU);
    if(new_fd < 0){
        throw E_UNKNOWN();
    }

    const int w_link = link(r_path.c_str(), w_path.c_str());
    if(w_link != 0){
        throw E_UNKNOWN();
    }

    const int d_link = link(r_path.c_str(), d_path.c_str());
    if(d_link != 0){
        throw E_UNKNOWN();
    }

    return;
}
