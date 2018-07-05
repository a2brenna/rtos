#include "fs_store.h"
#include <cassert>
#include <chrono>
#include <boost/algorithm/string.hpp>
#include <vector>

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "encode.h"

FS_Store::FS_Store(const std::string &path){
    _path = path;
}

FS_Store::~FS_Store(){
}

int FS_Store::_find_link(const R_Ref &id) const{
    return -1;
}

int FS_Store::_find_link(const W_Ref &id) const{
    return -1;
}

int FS_Store::_find_link(const D_Ref &id) const{
    return -1;
}
