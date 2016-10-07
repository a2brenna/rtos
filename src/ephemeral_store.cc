#include "ephemeral_store.h"

void Ephemeral_Store::store(const Id &id, const Data &data){
    _store[id] = data;
}

void Ephemeral_Store::append(const Id &id, const Data &data){
    _store[id].append(data);
}

Data Ephemeral_Store::fetch(const Id &id) const{
    try{
        return _store.at(id);
    }
    catch(std::out_of_range e){
        throw E_OBJECT_DNE();
    }
}
