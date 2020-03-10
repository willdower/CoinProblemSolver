#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <chrono>

int* findPrimes(int max) {
    //0 means prime
    //1 means non-prime
    int *primeArr = new int [max+1]{};

    //All even numbers after 2 are not primes
    for (int i=4;i<max;i+=2) {
        primeArr[i] = 1;
    }
    //All odd numbers up to the square root of max is to be tested for primality
    for (int i=3;i*i<max;i+=2) {
        //If they are already marked non-prime, skip
        if (primeArr[i] == 1) {
            continue;
        }
        //If a number is a prime, all multiples of it are not primes, so they are marked as such
        for (int j=i+i;j<max;j+=i) {
            primeArr[j] = 1;
        }
    }

    return primeArr;
}

int findWays(int iters, int soFar, int max, int goalAmount, int *primeArr, int minIters, int maxIters) {

    //If more than the max number of iterations, return
    if (iters > maxIters) {
        return 0;
    }

    //Don't want to add the prime to the chain on the first iteration - iteration 0 is the head of the root
    if (iters != 0) {
        soFar += max;
    }

    if (soFar == goalAmount) {
        //If not enough iterations, this is not a way
        if (iters < minIters) {
            return 0;
        }
        //If it is enough iterations, and not more than max as tested above, its a valid way
        else {
            return 1;
        }

    }
    else if (soFar > goalAmount) {
        //If adding another of this prime makes it go over, it is not a way
        return 0;
    }

    int totalWays = 0;

    for (int i=1;i<=max;i++) {
        //Filter out non-primes
        if (primeArr[i] == 1) {
            continue;
        }

        //Filter out chains that will end up less than the minimum number of iterations
        if ((goalAmount-soFar)-i < (minIters-iters)-1) {
            continue;
        }

        //Filter out chains that will end up more than the maximum number of iterations
        if (i*(maxIters-iters) < (goalAmount-soFar)) {
            continue;
        }

        totalWays += findWays(iters+1, soFar, i, goalAmount, primeArr, minIters, maxIters);
    }

    return totalWays;
}

int main() {

    int amount, minIters, maxIters;

    std::cin >> amount;
    std::cin >> minIters;
    std::cin >> maxIters;

    auto startExecution = std::chrono::system_clock::now();

    int *primeArr = findPrimes(amount);

    std::cout << findWays(0, 0, amount, amount, primeArr, minIters, maxIters);

    auto endExecution = std::chrono::system_clock::now();

    std::chrono::duration<double> timeTaken = endExecution - startExecution;

    std::cout << std::endl << timeTaken.count();

    delete [] primeArr;

    return 0;
}