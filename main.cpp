#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <sstream>

class node {
public:
    int branchPrime;
    int amountLeft;
    int degreesOfRemoval;
    int posToSet;
    bool posLocked;

    node(int prime, int n) {
        branchPrime = prime;
        amountLeft = n;
        degreesOfRemoval = 0;
        posToSet = -1;
        posLocked = false;
    }

    node(const node *oldNode, int prime) {
        branchPrime = prime;
        amountLeft = oldNode->amountLeft - prime;
        degreesOfRemoval = oldNode->degreesOfRemoval + 1;
        posToSet = oldNode->posToSet;
        posLocked = oldNode->posLocked;
    }
};

int* findPrimes(int max) {
    //0 means prime
    //1 means non-prime
    int *primeArr = new int [max+1]{};

    //All even numbers after 2 are not primes
    for (int i=4;i<=max;i+=2) {
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
        //If not enough or too many iterations, this is not a way -- BASE CASE
        if (iters < minIters || iters > maxIters) {
            return 0;
        }
        //If it is enough iterations, and not more than max as tested above, its a valid way -- BASE CASE
        else {
            return 1;
        }

    }
    else if (soFar > goalAmount) {
        //If adding another of this prime makes it go over, it is not a way -- BASE CASE
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

        //Do a further level of recursion
        totalWays += findWays(iters+1, soFar, i, goalAmount, primeArr, minIters, maxIters);
    }

    if (iters == 0 && minIters == 1 && primeArr[max] != 0) {
        totalWays += 1; //While primes will pick up themselves as the golden coin, non-primes will not, so added here
    }

    return totalWays;
}

int main(int argc, char **argv) {

    std::ifstream input;

    if (argc == 1) { //Check if filename was entered
        std::cout << "Filename required." << std::endl;
        return 0;
    }
    else {
        try {
            input.open(argv[1]);
        }
        catch (std::ifstream::failure & e){
            std::cout << "Failed to find or open input file." << std::endl;
            return 1;
        }
    }

    std::ofstream output;

    try {
        output.open("output.txt");
    }
    catch (std::ifstream::failure & e){
        std::cout << "Failed to create or open output file." << std::endl;
        return 1;
    }

    std::string line;

    while (std::getline(input, line)) { //Read problem line by line
        int amount, minIters = -1, maxIters = -1;

        std::stringstream lineSS(line);

        //Grab details from line
        lineSS >> amount;
        if (!lineSS.eof()) {
            lineSS >> minIters;
        }
        if (!lineSS.eof()) {
            lineSS >> maxIters;
        }

        if (minIters == -1) {
            minIters = 1;
            maxIters = amount;
        }
        else if (maxIters == -1) {
            maxIters = minIters;
        }

        auto startExecution = std::chrono::system_clock::now(); //Start time

        int *primeArr = findPrimes(amount); //Generate primes again every time to include it in running time for each one

        output << findWays(0, 0, amount, amount, primeArr, minIters, maxIters) << std::endl;

        auto endExecution = std::chrono::system_clock::now(); //End time

        std::chrono::duration<double> timeTaken = endExecution - startExecution;

        std::cout << std::endl << amount << " " << minIters << " " << maxIters << " - Time: " << timeTaken.count();

        delete [] primeArr;
    }

    return 0;
}