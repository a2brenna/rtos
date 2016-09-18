#ifndef __TYPES_H__
#define __TYPES_H__

#include <string>

//typedef std::string Id;
//typedef std::string Data;

class Id {

    public:

        Id();
        Id(const std::string &id);

        std::string id() const;

    private:

        std::string _id;


};

bool operator<(const Id &lhs, const Id &rhs);

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
