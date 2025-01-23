#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define constants for trends
#define SCRAP 0
#define NOMINAL 1
#define PRIMUS 2

int getRandomPrice(int generation_cost, int current_trend) {
    int min_price, max_price;

    switch (current_trend) {
        case SCRAP:
            min_price = 20;
            max_price = generation_cost - 10;
            break;
        case NOMINAL:
            min_price = 80;
            max_price = (int)(generation_cost * 1.05); // 105% of generation cost
            break;
        case PRIMUS:
            min_price = generation_cost;
            max_price = generation_cost * 4; // 400% of generation cost
            break;
        default:
            min_price = 0; // Default values (shouldn't happen)
            max_price = 0;
            break;
    }

    // Ensure the max price is not less than min price
    if (max_price < min_price) {
        max_price = min_price;
    }

    // Generate a random price between min_price and max_price
    return (rand() % (max_price - min_price + 1)) + min_price;
}

void displayStatus(int week, int day, int energon, int stacks_in_inventory) {
    printf("Energon Storage: %d    Stacks: %d\n", energon, stacks_in_inventory);
}

void generateCubes(int *energon, int *stacks_in_inventory) {
    int cost_per_cube = rand() % 41 + 80; 
    int stacks;
    char proceed;

    printf("Production cost for this week is %d Energon for 1 cube.\n", cost_per_cube);
    printf("It will cost %d Energon to produce 1 stack.\n", cost_per_cube * 10);
    printf("How many stacks do you wish to produce for this week? ");
    scanf("%d", &stacks);

    int total_cost = cost_per_cube * 10 * stacks;

    if (total_cost > *energon) {
        printf("Not enough Energon to produce that many stacks!\n");
        return;
    }

    printf("%d stacks will cost %d Energon, proceed? (y/n) ", stacks, total_cost);
    scanf(" %c", &proceed);
    if (proceed == 'y' || proceed == 'Y') {
        *energon -= total_cost;
        *stacks_in_inventory += stacks;
        printf("%d stacks produced.\n", stacks);
    } else {
        printf("Production canceled.\n");
    }
}

void sellCubes(int *energon, int *stacks_in_inventory, int generation_cost, int current_trend) {
    int price_per_cube;
    int stacks;
    char proceed;

    if (*stacks_in_inventory <= 0) {
        printf("No stacks available to sell.\n");
        return;
    }

    price_per_cube = getRandomPrice(generation_cost, current_trend);
    int sale_price_per_stack = price_per_cube * 10;

    printf("Swindle is buying Energon Cubes for %d Energon per cube.\n", price_per_cube);
    printf("You can earn %d Energon per stack.\n", sale_price_per_stack);
    printf("How many stacks do you wish to sell? ");
    scanf("%d", &stacks);

    if (stacks > *stacks_in_inventory) {
        printf("Not enough stacks in inventory.\n");
        return;
    }

    printf("%d stacks are about to be sold, proceed? (y/n) ", stacks);
    scanf(" %c", &proceed);
    if (proceed == 'y' || proceed == 'Y') {
        int total_sale = sale_price_per_stack * stacks;
        *energon += total_sale;
        *stacks_in_inventory -= stacks;
        printf("%d stacks sold.\n", stacks);
        printf("You earned %d Energon.\n", total_sale);
    } else {
        printf("Sale canceled.\n");
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
    char proceed; 

    srand(time(0));

    while (week <= 10) {
        printf("\n--- Week %d ---\n", week);
        displayStatus(week, 1, energon, stacks_in_inventory);

        // Generate cubes and set the generation cost for the week
        int generation_cost = rand() % 41 + 80; // Cost to generate a cube
        generateCubes(&energon, &stacks_in_inventory);

        // Determine the weekly trend randomly at the start of the week
        int current_trend = rand() % 3; // Randomly choose between SCRAP, NOMINAL, or PRIMUS

        for (int day = 1; day <= 7; day++) {
            printf("\nWeek %d Day %d\n", week, day);
            displayStatus(week, day, energon, stacks_in_inventory);

            sellCubes(&energon, &stacks_in_inventory, generation_cost, current_trend);

            if (energon >= 1000000) {
                printf("Congratulations! You have reached 1,000,000 Energon!\n");
                return 0;
            }

            if (energon < 800 && stacks_in_inventory == 0) {
                printf("Game over! Not enough Energon to produce more stacks or sell.\n");
                return 0;
            }
        }

        expireCubes(&stacks_in_inventory);
        week++;
    }

    printf("Game over. Total Energon: %d\n", energon);
    return 0;
}
