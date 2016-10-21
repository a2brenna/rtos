#include "ephemeral_store.h"

void Ephemeral_Store::store(const Ref&id, const Object &data){
    _store[id] = data;
}

void Ephemeral_Store::append(const Ref&id, const Object &data){
    _store[id].append(data);
}

Object Ephemeral_Store::fetch(const Ref&id) const{
    try{
        return _store.at(id);
    }
    catch(std::out_of_range e){
        throw E_OBJECT_DNE();
    }
}
