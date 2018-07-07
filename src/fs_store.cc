#include "fs_store.h"
#include <cassert>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/xattr.h>

#include "encode.h"

#include <iostream>

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

    //TODO: shorten w/ for loop
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

    //TODO: shorten w/ for loop
    //create_file
    const int new_fd = open(r_path.c_str(), O_CREAT | O_RDONLY, S_IRWXU);
    if(new_fd < 0){
        throw E_UNKNOWN();
    }

    const int r_xattr = fsetxattr(new_fd, "user.rtos.r_ref", read_id.base16().c_str(), 64, XATTR_CREATE);
    if(r_xattr != 0){
        std::cout << r_xattr << " " << errno << std::endl;
        throw E_UNKNOWN();
    }

    const int w_xattr = fsetxattr(new_fd, "user.rtos.w_ref", write_id.base16().c_str(), 64, XATTR_CREATE);
    if(w_xattr != 0){
        throw E_UNKNOWN();
    }

    const int d_xattr = fsetxattr(new_fd, "user.rtos.d_ref", rm_id.base16().c_str(), 64, XATTR_CREATE);
    if(d_xattr != 0){
        throw E_UNKNOWN();
    }

    close(new_fd);

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

void FS_Store::remove(const D_Ref &rm_id){

    const std::string d_path = _find_path(rm_id);

    //This is overkill for detecting if the file exists... but could be useful for online constistency checks
    /*
    struct stat statbuf;
    const int d_stat = stat(d_path.c_str(), &statbuf);
    if(r_stat == -1 || errno == ENOENT){
        throw E_OBJECT_DNE();
    }
    */

    char buff[65];

    const ssize_t r_ref_size = getxattr(d_path.c_str(), "user.rtos.r_ref", buff, 65);
    if(r_ref_size != 64){
        if(r_ref_size == -1 && errno == ENOENT){
            throw E_OBJECT_DNE();
        }
        else{
            throw E_UNKNOWN();
        }
    }
    const std::string r_path = _find_path(R_Ref(buff, BASE_16));
    std::cerr << r_path << std::endl;

    const ssize_t w_ref_size = getxattr(d_path.c_str(), "user.rtos.w_ref", buff, 65);
    if(w_ref_size != 64){
        if(w_ref_size == -1 && errno == ENOENT){
            throw E_OBJECT_DNE();
        }
        else{
            throw E_UNKNOWN();
        }
    }
    const std::string w_path = _find_path(W_Ref(buff, BASE_16));
    std::cerr << w_path << std::endl;

    const int r_unlink = unlink(r_path.c_str());
    if(r_unlink != 0){
        throw E_UNKNOWN();
    }

    const int w_unlink = unlink(w_path.c_str());
    if(w_unlink != 0){
        throw E_UNKNOWN();
    }

    const int d_unlink = unlink(d_path.c_str());
    if(d_unlink != 0){
        throw E_UNKNOWN();
    }

    return;

}
