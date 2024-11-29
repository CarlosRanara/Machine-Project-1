#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCRAP 0
#define NOMINAL 1
#define PRIMUS 2

int randomPrice(int generationCost, int currentTrends);
void displayStatus(int week, int day, int energon, int stacksInventory);
void generateCubes(int *energon, int *stacksInventory, int generationCost);
void sellCubes(int *energon, int *stacksInventory, int generationCost, int currentTrend);
void expireCubes(int *stacksInventory);

int main() {
    int energon = 10000, week = 1, stacksInventory = 0, generationCost = 0, currentTrend = 0, gameOver = 0, reachedGoal = 0;
    srand(time(0));

    while (week <= 10 && !gameOver) {
        if (energon < 800 && stacksInventory == 0) {
            printf("\nGame over! Not enough Energon to produce or sell more stacks.\n");
            gameOver = 1;
        } else {
            printf("\n--- Week %d Day 1 ---\n", week);
            displayStatus(week, 1, energon, stacksInventory);
            generationCost = rand() % 41 + 80;
            generateCubes(&energon, &stacksInventory, generationCost);
            currentTrend = rand() % 3;

            for (int day = 2; day <= 7 && !gameOver; day++) {
                printf("\n--- Week %d Day %d ---\n", week, day);
                displayStatus(week, day, energon, stacksInventory);
                sellCubes(&energon, &stacksInventory, generationCost, currentTrend);
                if (energon >= 1000000) {
                    reachedGoal = 1;
                    gameOver = 1;
                }
            }
            expireCubes(&stacksInventory);
            week++;
        }
    }

    if (reachedGoal) {
        printf("\nCongratulations! You reached 1,000,000 Energon!\n");
    } else {
        printf("\nGame over. Better luck next time!\n");
    }
    printf("Total Energon: %d\n", energon);
    return 0;
}

int randomPrice(int generationCost, int currentTrends) {
    int minPrice, maxPrice;
    switch (currentTrends) {
        case SCRAP: minPrice = 20; maxPrice = generationCost - 10; break;
        case NOMINAL: minPrice = 80; maxPrice = generationCost * 1.05; break;
        case PRIMUS: minPrice = generationCost; maxPrice = generationCost * 4; break;
        default: return -1;
    }
    return (rand() % (maxPrice - minPrice + 1)) + minPrice;
}

void displayStatus(int week, int day, int energon, int stacksInventory) {
    printf("Week %d Day %d | Energon Storage: %d | Stacks: %d\n", week, day, energon, stacksInventory);
}

void generateCubes(int *energon, int *stacksInventory, int generationCost) {
    int stacks;
    char proceed;
    printf("Production cost: %d Energon per cube. (1 stack = 10 cubes)\n", generationCost);
    while (1) {
        printf("Enter number of stacks to produce: ");
        if (scanf("%d", &stacks) == 1 && stacks >= 0) {
            int totalCost = stacks * generationCost * 10;
            if (totalCost <= *energon) {
                printf("Produce %d stacks for %d Energon? (y/n): ", stacks, totalCost);
                scanf(" %c", &proceed);
                if (proceed == 'y' || proceed == 'Y') {
                    *energon -= totalCost;
                    *stacksInventory += stacks;
                    printf("%d stacks produced.\n", stacks);
                    break;
                } else {
                    printf("Production canceled.\n");
                }
            } else {
                printf("Not enough Energon. Try a smaller amount.\n");
            }
        } else {
            printf("Invalid input. Please enter a non-negative number.\n");
            while (getchar() != '\n');
        }
    }
}

void sellCubes(int *energon, int *stacksInventory, int generationCost, int currentTrend) {
    if (*stacksInventory <= 0) {
        printf("No stacks to sell.\n");
        return;
    }
    int pricePerCube = randomPrice(generationCost, currentTrend), pricePerStack = pricePerCube * 10, stacks;
    char proceed;
    printf("Current market price: %d Energon per cube (%d per stack).\n", pricePerCube, pricePerStack);
    while (1) {
        printf("Enter number of stacks to sell: ");
        if (scanf("%d", &stacks) == 1 && stacks >= 0 && stacks <= *stacksInventory) {
            printf("Sell %d stacks for %d Energon? (y/n): ", stacks, stacks * pricePerStack);
            scanf(" %c", &proceed);
            if (proceed == 'y' || proceed == 'Y') {
                *energon += stacks * pricePerStack;
                *stacksInventory -= stacks;
                printf("%d stacks sold.\n", stacks);
                break;
            } else {
                printf("Sale canceled.\n");
            }
        } else {
            printf("Invalid input. Enter a number between 0 and %d.\n", *stacksInventory);
            while (getchar() != '\n');
        }
    }
}

void expireCubes(int *stacksInventory) {
    if (*stacksInventory > 0) {
        printf("All %d stacks expired at the end of the week.\n", *stacksInventory);
        *stacksInventory = 0;
    }
}
