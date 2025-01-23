#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define constants for trends
#define SCRAP 0
#define NOMINAL 1
#define PRIMUS 2
/* This function generates a random price based on the generation cost and current trend.
Precondition: generationCost is a non-negative integer and currentTrend is a valid trend constant.
@param generationCost the cost to generate a cube in Energon
@param currentTrend the current market trend (SCRAP, NOMINAL, or PRIMUS)
@return a random price within the calculated range based on the trend
*/
int getRandomPrice(int generation_cost, int current_trend) {
    int min_price, max_price;

    switch (current_trend) {
        case SCRAP:
            min_price = 20;
            max_price = generation_cost - 10;
            break;
        case NOMINAL:
            min_price = 80;
            max_price = (int)(generation_cost * 1.05);
            break;
        case PRIMUS:
            min_price = generation_cost;
            max_price = generation_cost * 4;
            break;
        default:
            printf("Error: Invalid market trend.\n");
            return -1; // Error indicator
    }

    if (max_price < min_price) {
        max_price = min_price;
    }

    return (rand() % (max_price - min_price + 1)) + min_price;
}
/* This function displays the current status of Energon storage and stacks in inventory.
Precondition: week, day, energon, and stacksInInventory are non-negative integers.
@param week the current week number
@param day the current day number
@param energon the amount of Energon available
@param stacksInInventory the number of stacks available in inventory
*/
void displayStatus(int week, int day, int energon, int stacks_in_inventory) {
    printf("Energon Storage: %d    Stacks: %d\n", energon, stacks_in_inventory);
}
/* This function generates Energon cubes based on user input and available Energon.
Precondition: energon and stacksInInventory are non-negative integers and generationCost is a non-negative integer.
@param energon pointer to the available Energon amount
@param stacksInInventory pointer to the number of stacks in inventory
@param generationCost the cost to generate a cube in Energon
*/
void generateCubes(int *energon, int *stacks_in_inventory, int generation_cost) {
    int stacks;
    char proceed = 'n';
    int valid_production = 0;

    printf("Production cost for this week is %d Energon for 1 cube.\n", generation_cost);
    printf("It will cost %d Energon to produce 1 stack.\n", generation_cost * 10);

    while (!valid_production) {
        printf("How many stacks do you wish to produce for this week? ");
        if (scanf("%d", &stacks) != 1 || stacks < 0) {
            printf("Invalid input. Please enter a non-negative integer.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }

        int total_cost = generation_cost * 10 * stacks;

        if (total_cost > *energon) {
            printf("Not enough Energon to produce that many stacks! Try a smaller amount.\n");
        } else {
            printf("%d stacks will cost %d Energon, proceed? (y/n) ", stacks, total_cost);
            while (getchar() != '\n'); // clear input buffer

            // Separate loop for confirming proceed
            do {
                scanf(" %c", &proceed);
                if (proceed == 'y' || proceed == 'Y') {
                    *energon -= total_cost;
                    *stacks_in_inventory += stacks;
                    printf("%d stacks produced.\n", stacks);
                    valid_production = 1;
                } else if (proceed == 'n' || proceed == 'N') {
                    printf("Production canceled. Let's try again.\n");
                    break;
                } else {
                    printf("Invalid choice. Please enter 'y' or 'n': ");
                    while (getchar() != '\n'); // clear input buffer
                }
            } while (proceed != 'y' && proceed != 'Y' && proceed != 'n' && proceed != 'N');
        }
    }
}   
/* This function handles the selling of Energon cubes based on user input.
Precondition: energon, stacksInInventory, and generationCost are non-negative integers,
and currentTrend is a valid trend constant.
@param energon pointer to the available Energon amount
@param stacksInInventory pointer to the number of stacks in inventory
@param generationCost the cost to generate a cube in Energon
@param currentTrend the current market trend (SCRAP, NOMINAL, or PRIMUS)
*/

void sellCubes(int *energon, int *stacks_in_inventory, int generation_cost, int current_trend) {
    if (*stacks_in_inventory <= 0) {
        printf("No stacks available to sell.\n");
        return;
    }

    int price_per_cube = getRandomPrice(generation_cost, current_trend);
    if (price_per_cube < 0) {
        printf("Sale aborted due to invalid market trend.\n");
        return;
    }

    int sale_price_per_stack = price_per_cube * 10;
    int stacks;
    char proceed = 'n';

    printf("Swindle is buying Energon Cubes for %d Energon per cube.\n", price_per_cube);
    printf("You can earn %d Energon per stack.\n", sale_price_per_stack);
    
    while (1) {
        printf("How many stacks do you wish to sell? ");
        if (scanf("%d", &stacks) != 1 || stacks < 0) {
            printf("Invalid input. Please enter a non-negative integer.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }

        if (stacks > *stacks_in_inventory) {
            printf("Not enough stacks in inventory. You have %d stacks available.\n", *stacks_in_inventory);
            continue;
        }

        printf("%d stacks are about to be sold, proceed? (y/n) ", stacks);
        while (getchar() != '\n'); // clear input buffer

        // Separate loop to handle confirmation
        do {
            scanf(" %c", &proceed);
            if (proceed == 'y' || proceed == 'Y') {
                int total_sale = sale_price_per_stack * stacks;
                *energon += total_sale;
                *stacks_in_inventory -= stacks;
                printf("%d stacks sold.\n", stacks);
                printf("You earned %d Energon.\n", total_sale);
                return;  // Exit after successful sale
            } else if (proceed == 'n' || proceed == 'N') {
                printf("Sale canceled.\n");
                return;  // Exit if sale is canceled
            } else {
                printf("Invalid choice. Please enter 'y' or 'n': ");
                while (getchar() != '\n'); // clear input buffer
            }
        } while (proceed != 'y' && proceed != 'Y' && proceed != 'n' && proceed != 'N');
    }
}

/* This function expires all remaining Energon cubes in inventory at the end of the week.
Precondition: stacksInInventory is a non-negative integer.
@param stacksInInventory pointer to the number of stacks in inventory
*/

void expireCubes(int *stacks_in_inventory) {
    if (*stacks_in_inventory > 0) {
        printf("\nExpiring Energon Cubes – The generated cubes expire at the end of the 7th day.\n");
        printf("%d stacks have expired.\n", *stacks_in_inventory);
        *stacks_in_inventory = 0;
    }
}

int main() {
    int energon = 10000;
    int week = 1;
    int stacks_in_inventory = 0;
    int game_over = 0;

    srand(time(0));

    while (week <= 10 && !game_over) {
        printf("\n--- Week %d Day 1 ---\n", week);
        displayStatus(week, 1, energon, stacks_in_inventory);

        int generation_cost = rand() % 41 + 80;
        generateCubes(&energon, &stacks_in_inventory, generation_cost);

        int current_trend = rand() % 3;
        printf("Trend for Week %d: %d\n", week, current_trend);

        for (int day = 2; day <= 7 && !game_over; day++) {
            printf("\n--- Week %d Day %d ---\n", week, day);
            displayStatus(week, day, energon, stacks_in_inventory);

            sellCubes(&energon, &stacks_in_inventory, generation_cost, current_trend);

            if (energon >= 1000000) {
                printf("Congratulations! You have reached 1,000,000 Energon!\n");
                game_over = 1;
            }

            if (energon < 800 && stacks_in_inventory == 0) {
                printf("Game over! Not enough Energon to produce more stacks or sell.\n");
                game_over = 1;
            }
        }

        if (!game_over) {
            expireCubes(&stacks_in_inventory);
            week++;
        }
    }

    if (!game_over) {
        printf("Game over. Total Energon: %d\n", energon);
    }
    return 0;
}
