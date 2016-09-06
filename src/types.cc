#include "types.h"

Id::Id(){
    //TODO: generate random Id
}

Id::Id(const std::string &id){
    _id = id;
}

std::string Id::id() const{
    return _id;
}

bool operator<(const Id &lhs, const Id &rhs){
    return lhs.id() < rhs.id();
}

/*
Data::Data(const std::string &data){
    _data = data;
}

std::string Data::data() const{
    return _data;
}
*/
