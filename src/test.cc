#include "ephemeral_store.h"
#include "leveldb_store.h"

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <boost/program_options.hpp>
#include <cassert>
#include <random>

namespace po = boost::program_options;

size_t NUM_TEST_OBJECTS = 128;
size_t OBJECT_MAX_SIZE = 64;
size_t MAX_STORAGE = 17179869184;
size_t KEY_LENGTH = 64;
double CORRECTNESS_COVERAGE = 1.00;

std::random_device rd;
std::mt19937 e2;

static const char hex[] =
    "0123456789abcdef";

static const char alphanum[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

std::string gen_random_hex(const size_t &len) {
	std::string s;

    for (size_t i = 0; i < len; ++i) {
        const char c = hex[rand() % (sizeof(alphanum) - 1)];
        s.append(1, c);
    }

	return s;
}

std::string gen_random_alphanum(const size_t &len) {
	std::string s;

    for (size_t i = 0; i < len; ++i) {
        const char c = alphanum[rand() % (sizeof(alphanum) - 1)];
        s.append(1, c);
    }

	return s;
}

std::string random_key(){
    return gen_random_hex(KEY_LENGTH);
}

void test(Object_Store *os, const std::vector<std::pair<Id, Data>> &data){
    const std::chrono::high_resolution_clock::time_point insert_start = std::chrono::high_resolution_clock::now();
    for(const auto &d: data){
        os->store(d.first, d.second);
    }
    const std::chrono::high_resolution_clock::time_point insert_end = std::chrono::high_resolution_clock::now();

    const std::chrono::high_resolution_clock::time_point retrieve_start = std::chrono::high_resolution_clock::now();
    for(const auto &d: data){
        volatile const auto foo = os->fetch(d.first).data();
    }
    const std::chrono::high_resolution_clock::time_point retrieve_end = std::chrono::high_resolution_clock::now();

    std::cout << "Insertion time: " << (insert_end - insert_start).count() << std::endl;
    std::cout << "Retrieval time: " << (retrieve_end - retrieve_start).count() << std::endl;
}

std::vector<std::pair<Id, Data>> generate_data(const size_t &max_size, const size_t &num_objects){
    std::vector<std::pair<Id, Data>> test_data;

    for(size_t i = 0; i < num_objects; i++){
        const size_t data_size = (rand() % (max_size- 1)) + 1;
        Id random_id(random_key());
        Data random_data(gen_random_alphanum(data_size));
        //std::cout << random_id.id().c_str() << " " << random_data.data().c_str() << std::endl;
        test_data.push_back(std::pair<Id, Data>(random_id, random_data));
    }

    return test_data;
}

void correctness_test(std::vector<std::pair<std::string,Object_Store *>> stores, const std::vector<std::pair<Id, Data>> &data, const double &coverage){
    std::uniform_real_distribution<> dist(0, 1);

    for(const auto &d: data){
        if( dist(e2) < CORRECTNESS_COVERAGE ){
            for(auto &s: stores){
                const auto stored = s.second->fetch(d.first);
                if (d.second == stored){
                    continue;
                }
                else{
                    std::cout << d.second.data() << " : " << stored.data() << std::endl;
                    std::cout << s.first << " failed" << std::endl;
                }
            }
        }
    }
}

int main(int argc, char* argv[]){

	po::options_description desc("Options");
	desc.add_options()
		("num_test_objects", po::value<size_t>(&NUM_TEST_OBJECTS), "Number of test objects to store and retrieve")
		("object_max_size", po::value<size_t>(&OBJECT_MAX_SIZE), "Maximum size of test objects")
		("max_storage", po::value<size_t>(&MAX_STORAGE), "Maximum number of bytes to test store")
		("correctness_coverage", po::value<double>(&CORRECTNESS_COVERAGE), "Portion of test data to use for correctness test")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    const size_t projected_storage = NUM_TEST_OBJECTS * (OBJECT_MAX_SIZE / 2);
    assert(projected_storage < MAX_STORAGE);

    e2 = std::mt19937(rd());

    std::vector<std::pair<Id, Data>> test_data = generate_data(OBJECT_MAX_SIZE, NUM_TEST_OBJECTS);

    std::vector<std::pair<std::string, Object_Store *>> stores;

    /* Test and time */

    Ephemeral_Store es;
    stores.push_back(std::pair<std::string, Object_Store *>("Ephemeral_Store",&es));
    test( &es, test_data );

    LevelDB_Store ls("example.ldb");
    stores.push_back(std::pair<std::string, Object_Store *>("LevelDB_Store ",&ls));
    test( &ls, test_data );

    correctness_test(stores, test_data, CORRECTNESS_COVERAGE);

    return 0;
};
