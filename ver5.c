#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define constants for trends
#define SCRAP 0
#define NOMINAL 1
#define PRIMUS 2

int getRandomPrice(int generation_cost, int current_trend) {
    int min_price, max_price;

    // Switch statement to determine price range based on trend
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
            return -1;  // Return early on invalid trend
    }

    return (rand() % (max_price - min_price + 1)) + min_price;
}

void displayStatus(int week, int day, int energon, int stacks_in_inventory) {
    printf("Energon Storage: %d    Stacks: %d\n", energon, stacks_in_inventory);
}


void generateCubes(int *energon, int *stacks_in_inventory, int generation_cost) {
    int stacks, valid_production = 0;
    char proceed;

    printf("Production cost for this week is %d Energon for 1 cube.\n", generation_cost);
    printf("It will cost %d Energon to produce 1 stack.\n", generation_cost * 10);

    while (valid_production == 0) {
        printf("How many stacks do you wish to produce for this week? ");
        
        if (scanf("%d", &stacks) == 1 && stacks >= 0) {
            int total_cost = generation_cost * 10 * stacks;

            if (total_cost <= *energon) {
                printf("%d stacks will cost %d Energon, proceed? (y/n) ", stacks, total_cost);
                getchar(); // Clear buffer
                scanf(" %c", &proceed);

                if (proceed == 'y' || proceed == 'Y') {
                    *energon -= total_cost;
                    *stacks_in_inventory += stacks;
                    printf("%d stacks produced.\n", stacks);
                    valid_production = 1;
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
            getchar(); // Clear buffer after invalid input
        }
    }
}


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
        while (valid_sale == 0) {
            printf("How many stacks do you wish to sell? ");
            
            // Input validation
            if (scanf("%d", &stacks) == 1 && stacks >= 0) { //scanf controversial easy fix if its not allowed get char maybe still dont fully understand
                if (stacks <= *stacks_in_inventory) {
                    printf("%d stacks are about to be sold, proceed? (y/n) ", stacks);
                    getchar();

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
                getchar(); // How tf do i use this shit 
            }
        }
    } else {
        if (*stacks_in_inventory <= 0) printf("No stacks available to sell.\n");
        if (price_per_cube < 0) printf("Sale aborted due to invalid market trend.\n");
    }
}


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
    int reached_million = 0;  // Track if 1 million Energon is reached

    srand(time(0));  // Seed random number generator

    // Loop through each week, up to 10 weeks or until game over
    while (week <= 10 && game_over == 0) 
    {
        printf("\n--- Week %d Day 1 ---\n", week);
        displayStatus(week, 1, energon, stacks_in_inventory);

        int generation_cost = rand() % 41 + 80;  // Random cost for cube generation
        generateCubes(&energon, &stacks_in_inventory, generation_cost);

        int current_trend = rand() % 3;  // Random trend for the week
        printf("Trend for Week %d: %d\n", week, current_trend); //PLEASE REMEMBER TO DELETE THIS IS FOR TESTING RAHHHH 

        // Daily operations from Day 2 to Day 7
        for (int day = 2; day <= 7 && game_over==0; day++) {
            printf("\n--- Week %d Day %d ---\n", week, day);
            displayStatus(week, day, energon, stacks_in_inventory);

            sellCubes(&energon, &stacks_in_inventory, generation_cost, current_trend);

            // Check for win or loss conditions
            if (energon >= 1000000) {
                reached_million = 1;  // Track that 1 million Energon has been reached
            }

            if (energon < 800 && stacks_in_inventory == 0) {
                printf("Game over! Not enough Energon to produce more stacks or sell.\n");
                game_over = 1;
            }
        }

        if (game_over==0) {
            expireCubes(&stacks_in_inventory);
            week++;  // Increment the week after expiration
        }
    }

    // After all weeks are completed, check if 1 million Energon was reached
    if (reached_million == 1) {
        printf("Congratulations! You reached 1,000,000 Energon during the game!\n");
    }

    // Simple Game Over message after 10 weeks or early game over
    printf("Game over. Total Energon: %d\n", energon);

    return 0;
}
