#ifndef __TYPES_H__
#define __TYPES_H__

#include <string>

enum ENCODING {
    RAW,
    BASE_16
};

class Ref {

    public:

        Ref();
        Ref(const std::string &id);
        Ref(const char *buf, const ENCODING &encoding);

        std::string base16() const;
        const char* buf() const;

    private:

        unsigned char _id[32];


};

class R_Ref : public Ref {

    public:

        R_Ref();
        R_Ref(const std::string &id);
        R_Ref(const char *buf, const ENCODING &encoding);

};

class W_Ref : public Ref {

    public:

        W_Ref();
        W_Ref(const std::string &id);
        W_Ref(const char *buf, const ENCODING &encoding);

};

class D_Ref : public Ref {

    public:

        D_Ref();
        D_Ref(const std::string &id);
        D_Ref(const char *buf, const ENCODING &encoding);

};

//bool operator<(const Ref &lhs, const Ref &rhs);

//TODO: Make a better type for this that supports a safe [] operator...?
//TODO: Make this support some sort of T extract<T>(const size_t &index) that safely checks size and casting to ensure you don't overrun the end when extracting type T from bytestring at position index
class Object {

    public:

        Object(const std::string &data);
        Object();

        void append(const Object &data);

        std::string data() const;

        const char* bytes() const;
        size_t size() const;

    private:

        std::string _data;

};

bool operator==(const Object &lfs, const Object &rhs);

bool operator!=(const Ref &lfs, const Ref &rhs);

#endif
