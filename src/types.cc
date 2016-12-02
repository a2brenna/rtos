#include "types.h"
#include <cassert>
#include <sodium.h>
#include <string.h>
#include "encode.h"

Ref::Ref(){
    randombytes_buf(_id, 32);
}

Ref::Ref(const std::string &id){
    assert(crypto_hash_sha256_BYTES == 32);
    crypto_hash_sha256(_id, (const unsigned char *)id.c_str(), id.size());
}

Ref::Ref(const char *buf, const size_t &len){
    assert(len == 32);
    memcpy(_id, buf, 32);
}

std::string Ref::base16() const{
    return base16_encode(_id, 32);
}

const char* Ref::buf() const{
    return (const char *)_id;
}

bool operator<(const Ref &lhs, const Ref &rhs){
    return ( strncmp(lhs.buf(), rhs.buf(), 32) <  0 );
}

Object::Object(){
}

Object::Object(const std::string &data){
    _data = data;
}

void Object::append(const Object &data){
    _data.append(data.data());
}

std::string Object::data() const{
    return _data;
}

bool operator==(const Object &lhs, const Object &rhs){
    return lhs.data() == rhs.data();
}
