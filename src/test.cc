#include "ephemeral_store.h"

#include <iostream>

int main(){

    Ephemeral_Store es;

    Id id("Test_Key");
    Data data("Test_Data");

    es.write(id, data);

    std::cout << es.read(id).data() << std::endl;

    return 0;
};
