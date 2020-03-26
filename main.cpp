#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <chrono>

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

int newFindWays(int n, const int *primeArr, const int min, const int max) {
    int ***waysArray = new int** [n+1];
    waysArray[0] = nullptr; //No use for 0

    for (int i=1;i<=n;i++) {
        for (int j = i; j > 0; j--) { //Grab largest prime below amountLeft
            //if (primeArr[j] == 0) {
                waysArray[i] = new int* [j+1];
                for (int k=1;k<=j;k++) {
                    waysArray[i][k] = new int [i]{0};
                }
                break;
            //}
        }
    }

    std::stack<node*> stack;

    for (int i=1;i<=n;i++) { //Main ways for each n loop

        stack.push(new node(i, n));

        while (!stack.empty()) {
            node *currentNode = stack.top();
            stack.pop();

            for (int j=1;j<=currentNode->branchPrime;j++) { //Loop for n in prime j and under -- j is branchPrime
                //Instead of referencing j for index, use posToSet, in case gone further down chain

                if (primeArr[j] != 0) {
                    continue;
                }

                if (currentNode->posToSet == -1) {
                    currentNode->posToSet = j;
                }
                /**else if (!currentNode->posLocked) {
                    currentNode->posToSet = j;
                    currentNode->posLocked = true;
                }**/

                if (j == 1 && !currentNode->posLocked) { //Making n in 1 and under - always 1
                    waysArray[i][i][i] = 1;
                    if (!currentNode->posLocked) {
                        currentNode->posToSet = -1;
                        currentNode->posLocked = false;
                    }
                    continue;
                }
                else if (j == i && !currentNode->posLocked) { //Making n in in prime n and under - always 1
                    waysArray[i][i][1] = 1;
                    if (!currentNode->posLocked) {
                        currentNode->posToSet = -1;
                        currentNode->posLocked = false;
                    }
                    continue;
                }

                if (currentNode->amountLeft-j > j) { //Originally just > then added right half of or
                    node *newNode = new node(currentNode, j);
                    newNode->posLocked = true;
                    stack.push(newNode);
                    continue;
                }
                else {
                    for (int k=1;k<=j;k++) {
                        if (currentNode->posToSet > currentNode->amountLeft - j) {
                            waysArray[i][currentNode->posToSet][k + currentNode->degreesOfRemoval + 1] =
                                    waysArray[currentNode->amountLeft - j][currentNode->amountLeft - j][k];
                        }
                        else {
                            waysArray[i][currentNode->posToSet][k + currentNode->degreesOfRemoval + 1] =
                                    waysArray[currentNode->amountLeft - j][currentNode->posToSet][k];
                        }
                    }
                }

                currentNode->posToSet = -1;
                currentNode->posLocked = false;
            }

        }
    }

    int ways = 0;
    /**int largestPrimeBelowN = 0, ways = 0;
    for (int i=n;i>0; i--) { //Grab largest prime below n
        if (primeArr[i] == 0) {
            largestPrimeBelowN = i;
            break;
        }
    }**/

    for (int i=1;i<=n;i++) {
        /**if (primeArr[i] != 0) {
            continue;
        }**/
        for (int j=min;j<=max;j++) {
            ways += waysArray[n][i][j];
        }
    }

    return ways;

}

int ripFindWays(int n, const int *primeArr) {
    int ways = 0;
    std::stack<node*> stack;
    int **waysArray = new int*[n];
    for (int i=0;i<n;i++) {
        waysArray[i] = nullptr;
    }

    for (int i=1;i<n;i++) {
        if (primeArr[i] == 1) {
            continue;
        }

        stack.push(new node(i, n));

        while (!stack.empty()) {
            node *currentNode = stack.top();
            stack.pop();
            for (int j=1;j<currentNode->branchPrime;j++) {
                if (primeArr[j] == 1) { //If j isn't prime, skip (only use prime j values)
                    continue;
                }
                if (currentNode->branchPrime > currentNode->amountLeft) {
                    for (int k=currentNode->amountLeft;k>0;k--) { //Grab largest prime below amountLeft
                        if (primeArr[k] == 0) {
                            currentNode->branchPrime = k;
                            break;
                        }
                    }
                }

                if (currentNode->amountLeft > currentNode->branchPrime) {
                    stack.push(new node(currentNode, j));
                    continue;
                }

                if (waysArray[currentNode->amountLeft] != nullptr) {
                    for (int k=0;k<=currentNode->branchPrime;k++) {
                        if (k <= 2) {
                            ways += 1;
                        }
                        else {
                            ways += waysArray[currentNode->amountLeft][k-2];
                        }
                    }
                }
                else {

                }

            }
            delete currentNode;
        }

    }

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

    //std::cout << findWays(0, 0, amount, amount, primeArr, minIters, maxIters);
    int ways = newFindWays(amount, primeArr, minIters, maxIters);
    std::cout << ways << std::endl;

    auto endExecution = std::chrono::system_clock::now();

    std::chrono::duration<double> timeTaken = endExecution - startExecution;

    std::cout << std::endl << timeTaken.count();

    delete [] primeArr;

    return 0;
}