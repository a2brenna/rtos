#include "ephemeral_store.h"

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <boost/program_options.hpp>
#include <cassert>

namespace po = boost::program_options;

static const char hex[] =
    "0123456789"
    "abcdef";

static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

std::string gen_random_hex(const size_t &len) {
	std::string s;

    for (int i = 0; i < len; ++i) {
        const char c = hex[rand() % (sizeof(alphanum) - 1)];
        s.append(1, c);
    }

	return s;
}

std::string gen_random_alphanum(const size_t &len) {
	std::string s;

    for (int i = 0; i < len; ++i) {
        const char c = alphanum[rand() % (sizeof(alphanum) - 1)];
        s.append(1, c);
    }

	return s;
}

size_t NUM_TEST_OBJECTS = 1024 * 1024;
size_t OBJECT_MAX_SIZE = 64;

int main(int argc, char* argv[]){

	po::options_description desc("Options");
	desc.add_options()
		("num_test_objects", po::value<size_t>(&NUM_TEST_OBJECTS), "Number of test objects to store and retrieve")
		("object_max_size", po::value<size_t>(&OBJECT_MAX_SIZE), "Maximum size of test objects")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    const size_t projected_storage = NUM_TEST_OBJECTS * (OBJECT_MAX_SIZE / 2);
    const size_t max_storage = 17179869184;

    assert(projected_storage < max_storage);

    std::vector<std::pair<Id, Data>> test_data;

    /* Generate a large amount of test data */

    for(size_t i = 0; i < NUM_TEST_OBJECTS; i++){
        const size_t data_size = (rand() % (OBJECT_MAX_SIZE - 1)) + 1;
        test_data.push_back(std::pair<Id, Data>(Id(gen_random_hex(64)), Data(gen_random_alphanum(data_size))));
    }

    /* Test and time */

    Ephemeral_Store es;

    const std::chrono::high_resolution_clock::time_point insert_start = std::chrono::high_resolution_clock::now();
    for(const auto &d: test_data){
        es.store(d.first, d.second);
    }
    const std::chrono::high_resolution_clock::time_point insert_end = std::chrono::high_resolution_clock::now();

    const std::chrono::high_resolution_clock::time_point retrieve_start = std::chrono::high_resolution_clock::now();
    for(const auto &d: test_data){
        volatile const auto foo = es.fetch(d.first).data();
    }
    const std::chrono::high_resolution_clock::time_point retrieve_end = std::chrono::high_resolution_clock::now();

    std::cout << "Insertion time: " << (insert_end - insert_start).count() << std::endl;
    std::cout << "Retrieval time: " << (retrieve_end - retrieve_start).count() << std::endl;

    return 0;
};
