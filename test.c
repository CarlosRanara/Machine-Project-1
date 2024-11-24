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

void testRandomPrice();
void testDisplayStatus();
void testGenerateCubes();
void testSellCubes();
void testExpireCubes();

int main() {
    srand(time(0));

    int mode = 0;

    while (1) {
        printf("Choose mode:\n");
        printf("1. Play game\n");
        printf("2. Run tests\n");
        printf("Enter your choice: ");
        if (scanf("%d", &mode) == 1 && (mode == 1 || mode == 2)) {
            break;
        } else {
            printf("Invalid input. Please enter 1 or 2.\n");
            while (getchar() != '\n');
        }
    }

    if (mode == 1) {
        int energon = 10000;
        int week = 1;
        int stacksInventory = 0;
        int gameOver = 0;
        int reachedGoal = 0;
        int generationCost = 0;
        int currentTrend = 0;
        char devMode;
        printf("Enter developer mode? (y/n): ");
        while (1) {
            if (scanf(" %c", &devMode) == 1 && (devMode == 'y' || devMode == 'Y' || devMode == 'n' || devMode == 'N')) {
                break;
            } else {
                printf("Invalid input. Please enter 'y' or 'n': ");
                while (getchar() != '\n');
            }
        }

        if (devMode == 'y' || devMode == 'Y') {
            printf("Enter starting Energon balance: ");
            while (scanf("%d", &energon) != 1 || energon < 0) {
                printf("Invalid input. Please enter a non-negative integer: ");
                while (getchar() != '\n');
            }

            printf("Enter starting week (1-10): ");
            while (scanf("%d", &week) != 1 || week < 1 || week > 10) {
                printf("Invalid week. Please enter a value between 1 and 10: ");
                while (getchar() != '\n');
            }
        }

        while (week <= 10 && gameOver == 0) {
            printf("\n --- Week %d Day 1 ---\n", week);
            displayStatus(week, 1, energon, stacksInventory);

             generationCost = rand() % 41 + 80;
            generateCubes(&energon, &stacksInventory, generationCost);

             currentTrend = rand() % 3;

            for (int day = 2; day <= 7; day++) {
                printf("\n --- Week %d Day %d ---\n", week, day);
                displayStatus(week, day, energon, stacksInventory);

                sellCubes(&energon, &stacksInventory, generationCost, currentTrend);

                if (energon >= 1000000) {
                    reachedGoal = 1;
                }
            }

            expireCubes(&stacksInventory);
            week++;

            if (reachedGoal) {
                printf("Congratulations! You reached 1,000,000 Energon during the game!\n");
                gameOver = 1;
            }
        }

        printf("Game over. Total Energon: %d\n", energon);
    } else if (mode == 2) {
        while (1) {
            int choice;
            printf("\n--- Testing Menu ---\n");
            printf("1. Test randomPrice\n");
            printf("2. Test displayStatus\n");
            printf("3. Test generateCubes\n");
            printf("4. Test sellCubes\n");
            printf("5. Test expireCubes\n");
            printf("6. Exit\n");
            printf("Select a function to test: ");
            if (scanf("%d", &choice) == 1 && choice >= 1 && choice <= 6) {
                if (choice == 6) {
                    printf("Exiting testing program.\n");
                    break;
                }
                switch (choice) {
                    case 1: testRandomPrice(); break;
                    case 2: testDisplayStatus(); break;
                    case 3: testGenerateCubes(); break;
                    case 4: testSellCubes(); break;
                    case 5: testExpireCubes(); break;
                }
            } else {
                printf("Invalid input. Please select a number between 1 and 6.\n");
                while (getchar() != '\n');
            }
        }
    }

    return 0;
}

// Function definitions for the main game logic and testing utilities remain unchanged...
void testRandomPrice() {
    int generationCost, currentTrends;
    printf("\n--- Testing randomPrice ---\n");
    printf("Enter generation cost: ");
    scanf("%d", &generationCost);
    printf("Enter current trend (0: SCRAP, 1: NOMINAL, 2: PRIMUS): ");
    scanf("%d", &currentTrends);

    int price = randomPrice(generationCost, currentTrends);
    printf("Generated price: %d\n", price);
}

void testDisplayStatus() {
    int week, day, energon, stacksInventory;
    printf("\n--- Testing displayStatus ---\n");
    printf("Enter week: ");
    scanf("%d", &week);
    printf("Enter day: ");
    scanf("%d", &day);
    printf("Enter energon balance: ");
    scanf("%d", &energon);
    printf("Enter stacks inventory: ");
    scanf("%d", &stacksInventory);

    displayStatus(week, day, energon, stacksInventory);
}

void testGenerateCubes() {
    int energon, stacksInventory, generationCost;
    printf("\n--- Testing generateCubes ---\n");
    printf("Enter energon balance: ");
    scanf("%d", &energon);
    printf("Enter stacks inventory: ");
    scanf("%d", &stacksInventory);
    printf("Enter generation cost per cube: ");
    scanf("%d", &generationCost);

    generateCubes(&energon, &stacksInventory, generationCost);
    printf("Updated energon balance: %d\n", energon);
    printf("Updated stacks inventory: %d\n", stacksInventory);
}

void testSellCubes() {
    int energon, stacksInventory, generationCost, currentTrend;
    printf("\n--- Testing sellCubes ---\n");
    printf("Enter energon balance: ");
    scanf("%d", &energon);
    printf("Enter stacks inventory: ");
    scanf("%d", &stacksInventory);
    printf("Enter generation cost per cube: ");
    scanf("%d", &generationCost);
    printf("Enter current trend (0: SCRAP, 1: NOMINAL, 2: PRIMUS): ");
    scanf("%d", &currentTrend);

    sellCubes(&energon, &stacksInventory, generationCost, currentTrend);
    printf("Updated energon balance: %d\n", energon);
    printf("Updated stacks inventory: %d\n", stacksInventory);
}

void testExpireCubes() {
    int stacksInventory;
    printf("\n--- Testing expireCubes ---\n");
    printf("Enter stacks inventory: ");
    scanf("%d", &stacksInventory);

    expireCubes(&stacksInventory);
    printf("Updated stacks inventory: %d\n", stacksInventory);
}

/* This function generates a random price for Energon cubes based on the 
 * production cost and the current market trend.
 * Precondition: generationCost must be a positive integer, and currentTrends must 
 * be one of the defined market trends (SCRAP, NOMINAL, or PRIMUS).
 * @param generationCost The cost to produce one Energon cube.
 * @param currentTrends The current market trend affecting the price.
 * @return A randomly generated price for a single Energon cube.
 */
/* This function generates a random price for Energon cubes based on the 
 * production cost and the current market trend.
 * Precondition: generationCost must be a positive integer, and currentTrends must 
 * be one of the defined market trends (SCRAP, NOMINAL, or PRIMUS).
 * @param generationCost The cost to produce one Energon cube.
 * @param currentTrends The current market trend affecting the price.
 * @return A randomly generated price for a single Energon cube.
 */
int randomPrice(int generationCost, int currentTrends) {
    int minPrice, maxPrice;

    switch (currentTrends) {
        case SCRAP:
            minPrice = 20;
            maxPrice = generationCost - 10;
            break;
        case NOMINAL:
            minPrice = 80;
            maxPrice = (generationCost * 105) / 100;
            break;
        case PRIMUS:
            minPrice = generationCost;
            maxPrice = generationCost * 4;
            break;
    }

    return (rand() % (maxPrice - minPrice + 1)) + minPrice;
}

/* This function displays the player's current Energon balance and inventory of Energon stacks.
 * Precondition: energon and stacksInventory must be non-negative integers.
 * @param week The current week number in the game.
 * @param day The current day number in the week.
 * @param energon The player's current Energon balance.
 * @param stacksInventory The number of Energon stacks in the player's inventory.
 */
void displayStatus(int week, int day, int energon, int stacksInventory) {
    printf("Energon Storage: %d     Stacks: %d\n", energon, stacksInventory);
}

/* This function prompts the player to decide how many stacks of Energon cubes to produce,
 * calculates the cost, and updates the player's Energon balance and inventory.
 * Precondition: generationCost must be a positive integer, and energon and stacksInventory must be valid.
 * @param energon Pointer to the player's current Energon balance.
 * @param stacksInventory Pointer to the player's current inventory of stacks.
 * @param generationCost The cost to produce one Energon cube.
 */
void generateCubes(int *energon, int *stacksInventory, int generationCost) {
    int stacks, validProduction = 0, total_cost = 0;
    char proceed;

    printf("Production cost for this week is %d Energon for 1 cube.\n", generationCost);
    printf("It will cost %d Energon to produce 1 stack.\n", generationCost * 10);

    while (validProduction == 0) {
            printf("How many stacks do you wish to produce for this week? ");
            scanf("%d", &stacks);
            
            if (stacks >= 0) {
                total_cost = generationCost * 10 * stacks;

                if (total_cost <= *energon) {
                    printf("%d stacks will cost %d Energon, proceed? (y/n) ", stacks, total_cost);
                    scanf(" %c", &proceed);

                    if (proceed == 'y' || proceed == 'Y') {
                        *energon -= total_cost;
                        *stacksInventory+= stacks;
                        printf("%d stacks produced.\n", stacks);
                        validProduction = 1;
                    } else if (proceed == 'n' || proceed == 'N') {
                        printf("Production canceled. Let's try again.\n");
                    } else {
                        printf("Invalid choice. Please enter 'y' or 'n'.\n");
                    }
                } else {
                    printf("Not enough Energon to produce that many stacks! Try a smaller amount.\n");
                }
            } else {
                printf("Invalid input. Please enter a non-negative integer.\n");
            
            }
        }
    }
/* This function allows the player to sell Energon cubes from their inventory at market prices.
 * Precondition: stacksInventory must be non-negative and the player must have cubes to sell.
 * @param energon Pointer to the player's current Energon balance.
 * @param stacksInventory Pointer to the player's current inventory of stacks.
 * @param generationCost The cost to produce one Energon cube.
 * @param currentTrend The current market trend affecting the selling price.
 */
void sellCubes(int *energon, int *stacksInventory, int generationCost, int currentTrend) {
    int pricePerCube = randomPrice(generationCost, currentTrend);
    int validSale = 0;
    int total_sale = 0;
    int salePricePerStack = 0;
    int stacks;
    char proceed;
    // Check if there are stacks to sell and a valid price
    if (*stacksInventory > 0 && pricePerCube >= 0) {
        salePricePerStack = pricePerCube * 10;
        printf("Swindle is buying Energon Cubes for %d Energon per cube.\n", pricePerCube);
        printf("You can earn %d Energon per stack.\n", salePricePerStack);

        // Loop until valid sale input is given
        while (validSale == 0) {
            printf("How many stacks do you wish to sell? ");
            scanf("%d", &stacks);
            // Input validation
            if (stacks >= 0) { 
                if (stacks <= *stacksInventory) {
                    printf("%d stacks are about to be sold, proceed? (y/n) ", stacks);
                    

                    scanf(" %c", &proceed);
                    if (proceed == 'y' || proceed == 'Y') {
                        // Calculate earnings and update inventory
                        total_sale = salePricePerStack * stacks;
                        *energon += total_sale;
                        *stacksInventory -= stacks;
                        printf("%d stacks sold.\n", stacks);
                        printf("You earned %d Energon.\n", total_sale);
                        validSale = 1;
                    } else if (proceed == 'n' || proceed == 'N') {
                        // User canceled sale
                        printf("Sale canceled.\n");
                        validSale = 0;
                    } else {
                        printf("Invalid choice. Please enter 'y' or 'n'.\n");
                    }
                } else {
                    printf("Not enough stacks in inventory. You have %d stacks available.\n", *stacksInventory);
                }
            } else {
                printf("Invalid input. Please enter a non-negative integer.\n");
                 
            }
        }
    } else {
        if (*stacksInventory <= 0) printf("No stacks available to sell.\n");
        if (pricePerCube < 0) printf("Sale aborted due to invalid market trend.\n");
    }
}



/* This function handles the expiration of Energon cubes at the end of the week (day 7).
 * All cubes in inventory expire at the end of the 7th day.
 * Precondition: stacksInventory must be a non-negative integer.
 * @param stacksInventory Pointer to the player's current inventory of stacks.
 */
void expireCubes(int *stacksInventory) {
    if (*stacksInventory > 0) {
        printf("\nExpiring Energon Cubes - The generated cubes expire at the end of the 7th day.\n");
        printf("%d stacks have expired.\n", *stacksInventory);
        *stacksInventory = 0;
    }
}
