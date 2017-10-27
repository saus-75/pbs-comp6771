#include "BucketSort.h"
#include <algorithm>
#include <cmath>
#include <string>

#include <thread>
#include <memory>

#include <iostream>

bool aLessB(const unsigned int& x, const unsigned int& y, unsigned int pow) {
    if (x == y){
        return false; // if the two numbers are the same then one is not less than the other
    }

    unsigned int a = x;
    unsigned int b = y;
    
    // work out the digit we are currently comparing on.
    if (pow == 0) {
        while (a / 10 > 0) {
            a = a / 10;
        }
        while (b / 10 > 0) {
            b = b / 10;
        } 
    } else {
        while (a / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
            a = a / 10;
        }
        while (b / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
            b = b / 10;
        } 
    }

    if (a == b){
        return aLessB(x,y,pow + 1);  // recurse if this digit is the same
    } else {
        return a < b;
    }
}

void t_sort(std::vector<unsigned int> &buckets){
    std::stable_sort(buckets.begin(), buckets.end(), [] (const unsigned int& x, const unsigned int& y)
        {
            return std::to_string(x) < std::to_string(y);
            // return aLessB(x,y,0);
        }
    );
}

// TODO: replace this with a parallel version.
void BucketSort::sort(unsigned int numCores) {
    std::vector<unsigned int> buckets[9];

    for (auto i: numbersToSort){
        std::string lex = std::to_string(i);
        int bn = (int)lex[0] - '0';
        buckets[bn - 1].push_back(i);
    }

    std::vector<std::shared_ptr<std::thread>> threads;

    for (int i = 0; i < 9; ++i){
        threads.push_back(std::make_shared<std::thread>(t_sort, std::ref(buckets[i])));
    }

    for (auto t : threads){
        t -> join();
    }

    std::vector<unsigned int> sv;
    for (int i = 0; i < 9; ++i){
        sv.insert(sv.end(), buckets[i].begin(), buckets[i].end());
    }

    numbersToSort.clear();
    numbersToSort.insert(numbersToSort.end(), sv.begin(), sv.end());

    // std::sort(numbersToSort.begin(), numbersToSort.end(), [] (const unsigned int& x, const unsigned int& y)
    //     {
    //         return aLessB(x,y,0);
    //     }
    // );
}