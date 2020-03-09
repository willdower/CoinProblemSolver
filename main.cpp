#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <chrono>

int* findPrimes(int max) {
    //0 means prime
    //1 means non-prime
    int *primeArr = new int [max+1]{};

    for (int i=4;i<max;i+=2) {
        primeArr[i] = 1;
    }

    for (int i=3;i*i<max;i+=2) {
        if (primeArr[i] == 1) {
            continue;
        }
        for (int j=i+i;j<max;j+=i) {
            primeArr[j] = 1;
        }
    }

    return primeArr;
}

int findWays(int iters, int soFar, int max, int goalAmount, int *primeArr, int minIters, int maxIters) {

    if (iters > maxIters) {
        return 0;
    }

    if (iters != 0) {
        soFar += max;
    }

    if (soFar == goalAmount) {
        if (iters < minIters) {
            return 0;
        }
        else {
            return 1;
        }

    }
    else if (soFar > goalAmount) {
        return 0;
    }

    int totalWays = 0;

    for (int i=1;i<=max;i++) {
        //Filter out non-primes
        if (primeArr[i] == 1) {
            continue;
        }

        if ((goalAmount-soFar)-i < (minIters-iters)-1) {
            continue;
        }

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