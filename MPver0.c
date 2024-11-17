#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define constants for trends
#define SCRAP 0
#define NOMINAL 1
#define PRIMUS 2

int randomPrice(int generationCost, int currentTrends)
{
    int minPrice, maxPrice;

    switch(currentTrends)
    {
        case SCRAP:
            minPrice = 20;
            maxPrice = generationCost - 10;
            break;
        case NOMINAL:
            minPrice = 80;
            maxPrice = (generationCost * 105)/100;
            break;
        case PRIMUS:
            minPrice = generationCost;
            maxPrice = generationCost * 4;
            break;
    }

    return (rand() % (maxPrice - minPrice + 1)) + minPrice;

}

void displayStatus(int week, int day, int energon, int stacksInventory)
{
    printf("Energon Storage: %d     Stacks: %d\n", energon, stacksInventory);
}

void generateCubes(int *energon, int *stacksInventory, int generationCost)
{
    int stacks, validProduction = 0;
    char proceed;

    printf("Production cost for this week is %d Energon for 1 cube.\n", generationCost);
    printf("It will cost %d Energon to produce 1 stack.\n", generationCost * 10);

    while (validProduction == 0)
    {
        printf("How many stacks do you wish to produce for this week? ");
        scanf("%d", &stacks);

        if (stacks >= 0)
        {
            int totalCost = generationCost * 10 * stacks;

            if (totalCost <= *energon)
            {
                printf("%d stacks will cost %d Energon, proceed? (y/n) ", stacks, totalCost);
                scanf(" %c", &proceed);

                if (proceed == 'Y' || proceed == 'y')
                {
                    *energon -= totalCost;
                    *stacksInventory += stacks;
                    printf("%d stacks produced.\n", stacks);
                    validProduction = 1;
                }

                else if (proceed == 'N' && proceed == 'n')
                    printf("Production canceled. Let's try again.\n");
                
                else
                    printf("Invalid choice. Please enter 'y' or 'n'.\n");    
            } 
            
            else 
                printf("Not enough Energon to produce that many stacks! Try a smaller amount.\n");
        }   
    
        else
            printf("Invalid input. Please enter a non-negative integer.\n");
    }

}

void sellCubes(int *energon, int *stacksInventory, int generationCost, int currentTrend)
{
    int pricePerCube = randomPrice(generationCost, currentTrend);
    int validSale = 0;

    if (*stacksInventory > 0 && pricePerCube >= 0)
    {
        int salePricePerStack = pricePerCube * 10;
        int stacks;
        char proceed;

        printf("Swindle is buying Energon Cubes for %d Energon per cube.\n", pricePerCube);
        printf("You can earn %d Energon per stack.\n", salePricePerStack);

        while (validSale == 0)
        {
            printf("How many stacks do you wish to sell? ");
            scanf("%d", &stacks);

            if (stacks >= 0)
            {
                if (stacks <= *stacksInventory)
                {
                    printf("%d stacks are about to be sold, proceed? (y/n) ", stacks);
                    scanf(" %c", &proceed);

                    if (proceed == 'y' || proceed == 'Y')
                    {
                        int totalSale = salePricePerStack * stacks;
                        *energon += totalSale;
                        *stacksInventory -= stacks;
                        printf("%d stacks sold.\n", stacks);
                        printf("You earned %d Energon.\n", totalSale);
                        validSale = 1;
                    }

                    else if (proceed == 'n' || proceed == 'N') 
                    {
                        printf("Sale canceled.\n");
                        validSale = 1;
                    }
                    
                    else
                        printf("Invalid choice. Please enter 'y' or 'n'.\n");
                }

                else 
                    printf("Not enough stacks in inventory. You have %d stacks available.\n", *stacksInventory);
        }   

        else 
            printf("Invalid input. Please enter a non-negative integer.\n");      
        }
    }
    
    else 
    {
        if (*stacksInventory <= 0) printf("No stacks available to sell.\n");
        if (pricePerCube < 0) printf("Sale aborted due to invalid market trend.\n");
    }
}


void expireCubes(int *stacksInventory)
{
      if (*stacksInventory > 0) 
      {
        printf("\nExpiring Energon Cubes - The generated cubes expire at the end of the 7th day.\n");
        printf("%d stacks have expired.\n", *stacksInventory);
        *stacksInventory = 0;
      }

}



int main(){
    int energon = 10000;
    int week = 1;
    int stacksInventory = 0;
    int gameOver = 0;
    int reachedGoal = 0;

    srand(time(0));

    while (week <= 10 && gameOver == 0)
    {
        printf("\n --- Week %d Day 1 ---\n", week);
        displayStatus( week, 1, energon, stacksInventory);

        int generationCost = rand() % 41 + 80;
        generateCubes(&energon, &stacksInventory, generationCost);

        int currentTrend = rand() % 3;
        printf("Trend for Week %d: %d\n", week, currentTrend); //REMOVE

        for (int day = 2; day <= 7; day++)
        {
            printf("\n --- Week %d Day %d ---\n", week, day);
            displayStatus( week, day, energon, stacksInventory);

            sellCubes(&energon, &stacksInventory, generationCost, currentTrend);
            
            if (energon >= 1000000)
            {
                reachedGoal = 1;
            }          
        }

        if (gameOver == 0)
        {
            expireCubes(&stacksInventory);
            week++;
        }  

        if(reachedGoal == 1)
        {
            printf("Congratulations! You reached 1,000,000 Energon during the game!\n");
        }
        
        printf("Game over. Total Energon: %d\n", energon);
    }

    return 0;
}
