#include "BucketSort.h"
#include <algorithm>
#include <cmath>
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
    std::sort(buckets.begin(), buckets.end(), [] (const unsigned int& x, const unsigned int& y)
        {
            return aLessB(x,y,0);
        }
    );
}

// TODO: replace this with a parallel version.
void BucketSort::sort(unsigned int numCores) {
    // std::sort(numbersToSort.begin(), numbersToSort.end(), [] (const unsigned int& x, const unsigned int& y)
    //     {
    //         return aLessB(x,y,0);
    //     }
    // );
    
    std::vector<unsigned int> buckets[numCores];

    unsigned int max_amount = numbersToSort.size()/numCores;
    std::cout << "Max values in each bucket: " << max_amount << "\n";

    int curr_bucket = 0;

    for (auto i: numbersToSort){
        if (buckets[curr_bucket].size() < max_amount){
            buckets[curr_bucket].push_back(i);
        } else {
            buckets[curr_bucket].push_back(i);
            std::cout << buckets[curr_bucket].size() << "\n";
            curr_bucket++;
        }
    }
    std::cout << buckets[curr_bucket].size() << "\n";

    std::vector<std::shared_ptr<std::thread>> threads;

    for (auto i = 0U; i < numCores; ++i){
        threads.push_back(std::make_shared<std::thread>(t_sort, std::ref(buckets[i])));
    }

    for (auto t : threads){
        t -> join();
    }

    std::vector<unsigned int> sv;
    for (auto i = 0U; i < numCores; ++i){
        sv.insert(sv.end(), buckets[i].begin(), buckets[i].end());
    }

    std::sort(sv.begin(),sv.end(), [] (const unsigned int& x, const unsigned int& y){
            return aLessB(x,y,0);
    } );

    numbersToSort.clear();
    numbersToSort.insert(numbersToSort.end(), sv.begin(), sv.end());
}