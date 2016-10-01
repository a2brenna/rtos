#include "types.h"
#include <cassert>
#include <sodium.h>
#include <string.h>
#include "encode.h"

Id::Id(){
    randombytes_buf(_id, 32);
}

Id::Id(const std::string &id){
    assert(crypto_hash_sha256_BYTES == 32);
    crypto_hash_sha256(_id, (const unsigned char *)id.c_str(), id.size());
}

std::string Id::id() const{
    return base16_encode(_id, 32);
}

const char* Id::buf() const{
    return (const char *)_id;
}

bool operator<(const Id &lhs, const Id &rhs){
    return ( strncmp(lhs.buf(), rhs.buf(), 32) <  0 );
}

Data::Data(){
}

Data::Data(const std::string &data){
    _data = data;
}

void Data::append(const Data &data){
    _data.append(data.data());
}

std::string Data::data() const{
    return _data;
}

bool operator==(const Data &lhs, const Data &rhs){
    return lhs.data() == rhs.data();
}
