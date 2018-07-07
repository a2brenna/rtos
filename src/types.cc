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

Ref::Ref(const char *buf, const ENCODING &encoding){
    if(encoding == RAW){
        memcpy(_id, buf, 32);
    }
    else if(encoding == BASE_16){
        const std::string bytes = base16_decode(buf, 64);
        memcpy(_id, bytes.c_str(), 32);
    }
    else{
        assert(false);
    }
}

std::string Ref::base16() const{
    return base16_encode((const char *)_id, 32);
}

const char* Ref::buf() const{
    return (const char *)_id;
}

bool operator<(const Ref &lhs, const Ref &rhs){
    return ( strncmp(lhs.buf(), rhs.buf(), 32) <  0 );
}

bool operator!=(const Ref &lhs, const Ref &rhs){
    return ( strncmp(lhs.buf(), rhs.buf(), 32) !=  0 );
}

R_Ref::R_Ref() :
    Ref()
{

}

R_Ref::R_Ref(const std::string &name) :
    Ref(name)
{

}

R_Ref::R_Ref(const char *buf, const ENCODING &encoding) :
    Ref(buf, encoding)
{

}

W_Ref::W_Ref() :
    Ref()
{

}

W_Ref::W_Ref(const std::string &name) :
    Ref(name)
{

}

W_Ref::W_Ref(const char *buf, const ENCODING &encoding) :
    Ref(buf, encoding)
{

}

D_Ref::D_Ref() :
    Ref()
{

}

D_Ref::D_Ref(const std::string &name) :
    Ref(name)
{

}

D_Ref::D_Ref(const char *buf, const ENCODING &encoding) :
    Ref(buf, encoding)
{

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

const char* Object::bytes() const{
    return _data.c_str();
}

size_t Object::size() const{
    return _data.size();
}

bool operator==(const Object &lhs, const Object &rhs){
    return lhs.data() == rhs.data();
}
