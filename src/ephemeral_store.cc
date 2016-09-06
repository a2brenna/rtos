#include "ephemeral_store.h"

void Ephemeral_Store::write(const Id &id, const Data &data){
    _store[id] = data;
}

Data Ephemeral_Store::read(const Id &id) const{
    try{
        return _store.at(id);
    }
    catch(std::out_of_range e){
        return Data("");
    }
}
