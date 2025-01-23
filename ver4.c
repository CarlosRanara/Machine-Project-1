#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define constants for trends
#define SCRAP 0
#define NOMINAL 1
#define PRIMUS 2

/* Generates a random price based on the generation cost and current trend.
   If the trend is invalid, returns -1 as an error indicator.
   @param generation_cost the cost to generate a cube in Energon
   @param current_trend the current market trend (SCRAP, NOMINAL, or PRIMUS)
   @return a random price within the range based on the trend */
int getRandomPrice(int generation_cost, int current_trend) {
    int min_price, max_price;

    // Determine price range based on trend
    if (current_trend == SCRAP) {
        min_price = 20;
        max_price = generation_cost - 10;
    } else if (current_trend == NOMINAL) {
        min_price = 80;
        max_price = (int)(generation_cost * 1.05);
    } else if (current_trend == PRIMUS) {
        min_price = generation_cost;
        max_price = generation_cost * 4;
    } else {
        printf("Error: Invalid market trend.\n");
        min_price = -1;  // Indicate error if trend is invalid
        max_price = -1;
    }

    // Ensure minimum price doesn't exceed maximum
    if (max_price < min_price) {
        max_price = min_price;
    }

    return (min_price >= 0) ? (rand() % (max_price - min_price + 1)) + min_price : -1;
}

/* Displays the current status of Energon storage and stacks in inventory.
   @param week the current week number
   @param day the current day number
   @param energon the amount of Energon available
   @param stacks_in_inventory the number of stacks available in inventory */
void displayStatus(int week, int day, int energon, int stacks_in_inventory) {
    printf("Energon Storage: %d    Stacks: %d\n", energon, stacks_in_inventory);
}

/* Generates Energon cubes based on user input, deducting Energon cost from storage.
   If the user doesn't have enough Energon or chooses to cancel, they can try again.
   @param energon pointer to the available Energon amount
   @param stacks_in_inventory pointer to the number of stacks in inventory
   @param generation_cost the cost to generate a cube in Energon */
void generateCubes(int *energon, int *stacks_in_inventory, int generation_cost) {
    int stacks, valid_production = 0;
    char proceed;

    printf("Production cost for this week is %d Energon for 1 cube.\n", generation_cost);
    printf("It will cost %d Energon to produce 1 stack.\n", generation_cost * 10);

    // Loop until valid production input is given
    while (!valid_production) {
        printf("How many stacks do you wish to produce for this week? ");
        
        // Input validation
        if (scanf("%d", &stacks) == 1 && stacks >= 0) {
            int total_cost = generation_cost * 10 * stacks;

            // Check if user has enough Energon for production
            if (total_cost <= *energon) {
                printf("%d stacks will cost %d Energon, proceed? (y/n) ", stacks, total_cost);
                while (getchar() != '\n'); // clear input buffer
                
                scanf(" %c", &proceed);
                if (proceed == 'y' || proceed == 'Y') {
                    // Deduct cost and add stacks if user confirms
                    *energon -= total_cost;
                    *stacks_in_inventory += stacks;
                    printf("%d stacks produced.\n", stacks);
                    valid_production = 1;
                } else if (proceed == 'n' || proceed == 'N') {
                    // User canceled production
                    printf("Production canceled. Let's try again.\n");
                } else {
                    printf("Invalid choice. Please enter 'y' or 'n'.\n");
                }
            } else {
                printf("Not enough Energon to produce that many stacks! Try a smaller amount.\n");
            }
        } else {
            printf("Invalid input. Please enter a non-negative integer.\n");
            while (getchar() != '\n'); // clear input buffer
        }
    }
}

/* Handles selling Energon cubes based on user input, adding earnings to storage.
   If the sale is successful, deducts sold stacks from inventory.
   @param energon pointer to the available Energon amount
   @param stacks_in_inventory pointer to the number of stacks in inventory
   @param generation_cost the cost to generate a cube in Energon
   @param current_trend the current market trend (SCRAP, NOMINAL, or PRIMUS) */
void sellCubes(int *energon, int *stacks_in_inventory, int generation_cost, int current_trend) {
    int price_per_cube = getRandomPrice(generation_cost, current_trend);
    int valid_sale = 0;

    // Check if there are stacks to sell and a valid price
    if (*stacks_in_inventory > 0 && price_per_cube >= 0) {
        int sale_price_per_stack = price_per_cube * 10;
        int stacks;
        char proceed;

        printf("Swindle is buying Energon Cubes for %d Energon per cube.\n", price_per_cube);
        printf("You can earn %d Energon per stack.\n", sale_price_per_stack);

        // Loop until valid sale input is given
        while (!valid_sale) {
            printf("How many stacks do you wish to sell? ");
            
            // Input validation
            if (scanf("%d", &stacks) == 1 && stacks >= 0) {
                if (stacks <= *stacks_in_inventory) {
                    printf("%d stacks are about to be sold, proceed? (y/n) ", stacks);
                    while (getchar() != '\n'); // clear input buffer

                    scanf(" %c", &proceed);
                    if (proceed == 'y' || proceed == 'Y') {
                        // Calculate earnings and update inventory
                        int total_sale = sale_price_per_stack * stacks;
                        *energon += total_sale;
                        *stacks_in_inventory -= stacks;
                        printf("%d stacks sold.\n", stacks);
                        printf("You earned %d Energon.\n", total_sale);
                        valid_sale = 1;
                    } else if (proceed == 'n' || proceed == 'N') {
                        // User canceled sale
                        printf("Sale canceled.\n");
                        valid_sale = 1;
                    } else {
                        printf("Invalid choice. Please enter 'y' or 'n'.\n");
                    }
                } else {
                    printf("Not enough stacks in inventory. You have %d stacks available.\n", *stacks_in_inventory);
                }
            } else {
                printf("Invalid input. Please enter a non-negative integer.\n");
                while (getchar() != '\n'); // clear input buffer
            }
        }
    } else {
        if (*stacks_in_inventory <= 0) printf("No stacks available to sell.\n");
        if (price_per_cube < 0) printf("Sale aborted due to invalid market trend.\n");
    }
}

/* Expires all remaining Energon cubes at the end of the week.
   @param stacks_in_inventory pointer to the number of stacks in inventory */
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

    srand(time(0));  // Seed random number generator

    // Loop through each week, up to 10 weeks or until game over
    while (week <= 10 && !game_over) {
        printf("\n--- Week %d Day 1 ---\n", week);
        displayStatus(week, 1, energon, stacks_in_inventory);

        int generation_cost = rand() % 41 + 80;  // Random cost for cube generation
        generateCubes(&energon, &stacks_in_inventory, generation_cost);

        int current_trend = rand() % 3;  // Random trend for the week
        printf("Trend for Week %d: %d\n", week, current_trend);

        // Daily operations from Day 2 to Day 7
        for (int day = 2; day <= 7 && !game_over; day++) {
            printf("\n--- Week %d Day %d ---\n", week, day);
            displayStatus(week, day, energon, stacks_in_inventory);

            sellCubes(&energon, &stacks_in_inventory, generation_cost, current_trend);

            // Check for win or loss conditions
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
