#ifndef __TYPES_H__
#define __TYPES_H__

#include <string>

class Id {

    public:

        Id();
        Id(const std::string &id);
        Id(const char *buf, const size_t &len);

        std::string base16() const;
        const char* buf() const;

    private:

        unsigned char _id[32];


};

bool operator<(const Id &lhs, const Id &rhs);

//TODO: Make a better type for this that supports a safe [] operator...?
//TODO: Make this support some sort of T extract<T>(const size_t &index) that safely checks size and casting to ensure you don't overrun the end when extracting type T from bytestring at position index
class Data {

    public:

        Data(const std::string &data);
        Data();

        void append(const Data &data);

        std::string data() const;

    private:

        std::string _data;

};

bool operator==(const Data &lfs, const Data &rhs);

#endif
