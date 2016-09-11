#include "ephemeral_store.h"

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

int main(){

    std::vector<std::pair<Id, Data>> test_data;

    /* Generate a large amount of test data */
    test_data.push_back(std::pair<Id, Data>(Id("Test_Key"), Data("Test_Data")));

    /* Test and time */

    Ephemeral_Store es;

    for(const auto &d: test_data){
        es.write(d.first, d.second);
    }

    for(const auto &d: test_data){
        std::cout << es.read(d.first).data() << std::endl;
    }

    return 0;
};
