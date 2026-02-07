#ifndef UNTITLED4_RANDOM_H
#define UNTITLED4_RANDOM_H
#include <random>
#include <chrono>
namespace Random {
    inline std::mt19937 generate() {
        std::random_device rd{};
        std::seed_seq ss{static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
                rd(), rd(), rd(), rd(), rd(), rd(), rd()};
        return std::mt19937{ss};
    }
    inline std::mt19937 mt{generate()};
}
#endif //UNTITLED4_RANDOM_H