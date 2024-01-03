#include "utils.h"

int createRandomNumbers(int start,int end){
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the range of random numbers (0 to 255)
    std::uniform_int_distribution<int> distribution(start,end);

    // Generate a random integer
    int randomValue = distribution(gen);

    return randomValue;
}