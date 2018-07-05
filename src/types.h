#ifndef __TYPES_H__
#define __TYPES_H__

#include <string>

class Ref {

    public:

        Ref();
        Ref(const std::string &id);
        Ref(const char *buf, const size_t &len);

        std::string base16() const;
        const char* buf() const;

    private:

        unsigned char _id[32];


};

class R_Ref : public Ref {};
class W_Ref : public Ref {};
class D_Ref : public Ref {};

//bool operator<(const Ref &lhs, const Ref &rhs);

//TODO: Make a better type for this that supports a safe [] operator...?
//TODO: Make this support some sort of T extract<T>(const size_t &index) that safely checks size and casting to ensure you don't overrun the end when extracting type T from bytestring at position index
class Object {

    public:

        Object(const std::string &data);
        Object();

        void append(const Object &data);

        std::string data() const;

    private:

        std::string _data;

};

bool operator==(const Object &lfs, const Object &rhs);

#endif
