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

int FS_Store::_find_link(const enum REF_TYPE, const Ref &id) const{
    return -1;
}
