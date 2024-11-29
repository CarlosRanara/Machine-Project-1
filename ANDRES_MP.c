/**************************************************************
*************
This is to certify that this project is my own work, based on my personal efforts
in studying and applying the concepts learned. I have constructed the
functions and their respective algorithms and corresponding code by myself.
The program was run, tested, and debugged by my own efforts. I further
certify that I have not copied in part or whole or otherwise plagiarized the
work of other students and/or persons.
 Andres, Sei Shekinah A., DLSU ID 12411051
***************************************************************
************/

/*
	Description: Energon Cubes Production.
				Generates cubes on the first day, then
				sell the generated cubes on the second to
				seventh day until the goal of 1 Million
				energon is achieved.
	Programmed by: Andres, Sei Shekinah A.  Section: S24A
	Last modified: 11/24/2024
	Version: 5.3
	Acknowledgements: stdio.h, stdlib.h, time.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TARGET_ENERGON 1000000 // Constant value of energon to be achieved in the game
#define MAX_WEEKS 10 // Constant value of the amount of maximum weeks in the game

// Function Prototypes
int generateEnergonCost ();
int randomTrend ();
int getDailyPrice (int basePrice, int trend);
void askStocks (int costPerCube, int* availableEnergon, int* stacksProduced);
void confirmProd (int totalCost, int stacks, int* confirm);
void updateEnergonStorage (int* energonStorage, int amount);
void devMode (int* week, int* energonStorage, int* devTrend);

// ----------------------------------------------------------

/*
	Generate the random energon cost per week between 80 to 120, then return the randomized value.
	
	@return randomized energon cost value between 80 to 120
*/

int generateEnergonCost () {
	return 80 + rand () % 41;
}

/*
	Generate a value between 0 to 2, where each number corresponds to specified trends, then return the randomized value.
	
	Precondition:
	
	@return randomized energon cost value between 0 to 2
*/

int randomTrend () {
	return rand () % 3; 
}

// -----------------------------------------------------------------------------------

/*
	Specifies the trends and which number they correspond to from randomTrend
	then calculate the amount of price within a certain range depending on the trend given.
	
	0 = Scrap trend: minimum 20 energon, maximum energon is equivalent to generation cost - 10
	1 = Nominal trend: minimum 80 energon, maximum energon is equivalent to 105% of generation cost
	2 = Primus trend: minimum is equivalent to generation cost, maximum is equivalent to 400% of generation cost
	
	@param basePrice - minimum price for the week
	@param trend - scrap, nominal or primus trend
	@param price - value after the trend modification, value to be returned
	@param maxPrice - maximum price for the week
	
	@return price value for the day
*/

int getDailyPrice (int basePrice, int trend) {
	int price, maxPrice;
	
	if (trend == 0) { 
		maxPrice = basePrice - 10;
		price = 20 + rand() % (maxPrice - 20 + 1); 
	}
	else if (trend == 1) {
		maxPrice = basePrice + (basePrice * 5) / 100;
		price = 80 + rand() % (maxPrice - 80 + 1);
	} 
	else if (trend == 2) {
		maxPrice = basePrice * 4;
		price = basePrice + rand() % (maxPrice - basePrice + 1);
	}
	return price;
}	

/*
	Ask the user how many stacks they want to ask for the week. If the user an invalid value,
	then the function will loop back to the first question.
	
	Precondition: user inputs 2 in the title screen or user has finished modifying all values in dev mode
	
	@param costPerCube - Amount of energon to produce 1 cube
	@param *availableEnergon - ampint of available energon in storage
	@param *stacksProduced - amount of stacks produced for the week
	@param stacks - amount of stacks in storage
	@param totalCost - calculate total cost amount of energon to produce the specified number of stacks based from user input
	@param confirm - Checks if user has confirmed their input
	@param costPerSTacktotal - amount of energon needed to produce one stack
	
	@return none
*/

void askStocks (int costPerCube, int* availableEnergon, int* stacksProduced) {
	int stacks = 0, totalCost = 0, confirm = 0;
	int costPerStackTotal = 10 * costPerCube;
	
	while (confirm == 0) {
		printf("\nProduction cost for this week is %d energon for 1 cube.\n", costPerCube);
		printf("It will cost %d Energon to produce 1 stack.\n\n", costPerStackTotal);
	
		printf("How many stacks do you wish to produce for this week?");
		scanf("%d", &stacks);
		
		if (stacks< 0)
			printf("Invalid number of stacks! Please input a positive number or 0.\n\n");
		else {
			totalCost = stacks * costPerStackTotal;
			if (totalCost > *availableEnergon) {
				printf("Not enough Energon to produce %d stacks! You only have %d energon available.\n\n", stacks, *availableEnergon);
			} else
				confirmProd(totalCost, stacks, &confirm);
		}
	}
	*stacksProduced = stacks;
}

/*
	Confirm the amount of stacks the user will produce. It will loop back to the question,
	if the user inputs n or N.
	
	@param totalCost - calculate total cost amount of energon to produce the specified number of stacks based from user input
	@param stacks - amount of stacks in storage
	@param confirm - Checks if user has confirmed their input
	@param ynAns - Yes or No input
	
	@return none
*/

void confirmProd (int totalCost, int stacks, int* confirm) {
	char ynAns;
	
	printf("%d stacks will cost %d energon, proceed? (y/n) \n", stacks, totalCost);
	scanf(" %c", &ynAns);
	
	if (ynAns == 'y' || ynAns == 'Y') {
		*confirm = 1;
		printf("%d stacks produced.\n \n", stacks);
	} else
		printf("Production Cancelled. Please try again. \n\n");
}

/*
	Update the energon storage per week.
	
	Precondition: none
	
	@param energonStorage - Amount of energon the user has
	@amount - updates amount of energon by adding and avoid redundancy
	
	@return none
*/

void updateEnergonStorage (int* energonStorage, int amount) {
	*energonStorage += amount;
}

/*
	If the user chose to enter dev mode, they will be lead to this screen and
	will be asked how many features they wish to modify. Then, it will ask the
	user which feature they want to modify.
	
	Precondition: ifDevMode = 1 / User chose 1 (Dev Mode) in the title screen
	
	@param energonStorage - Amount of energon the user has
	@param week - Value for the current week
	@param cubesInStorage - Amount of cubes in storage
	@param modifyOption - How many features the user wants to modify
	@param choice - Which feature user wants to modify
	@param firstChoice - Which feature user wants to modify first
	@param secondChoice - which feature user wants to modify second
	
	@return none
*/

void devMode (int* week, int* energonStorage, int* devTrend) {
	int modifyOption = 0, choice = 0, firstChoice = 0, secondChoice = 0;
	
	printf("Dev mode selected!\n\n");
	
	do {
		printf ("How many features would you like to modify? (1 - 3 features): ");
		scanf("%d", &modifyOption);
		if(modifyOption < 1 || modifyOption > 3)
			printf("Invalid choice. Please pick a number between 1 and 3.\n");
	} while(modifyOption < 1 || modifyOption > 3);
	
	
	switch (modifyOption) {
		case 1: {
			do {
				printf("Which feature would you like to modify?\n\n");
				printf("1.Week\n2.Energon\n3.Trend\n ");
				scanf("%d", &choice);
				if (choice < 1 || choice > 3)
					printf("Invalid choice. Please pick  number between 1 to 3.\n");
			} while (choice < 1 || choice > 3);
		
			switch (choice) {
				case 1:
					do {
						printf("Please enter the week: (1 - %d)\n\n", MAX_WEEKS);
						scanf("%d", week);
						if(*week < 1 || *week > MAX_WEEKS)
							printf("Invalid number. Please input a number between 1 to 10\n");
					} while (*week < 1 || *week > MAX_WEEKS);
					break;
					
				case 2:
					do {
						printf("Please enter the amount of energon in storage: \n\n");
						scanf("%d", energonStorage);
						if (*energonStorage < 0)
							printf("Invalid number. Please input a positive number.\n");
					} while (*energonStorage < 0);
					break;
					
				case 3:
					do {
						printf("Please select the trend: \n");
						printf("0.) Scrap\n1.) Nominal\n2.) Primus\n\n");
						scanf("%d", devTrend);
						if (*devTrend < 0 || *devTrend > 2)
							printf("Invalid number. Please input a number between 0 to 2.\n");
					} while (*devTrend < 0 || *devTrend > 2);
					break;
					
				default:
					printf("Invalid Choice. \n");
			}
			break;
		}
		case 2: {
			do{
				printf("Which feature would you like to modify first?\n\n");
				printf("1.Week\n2.Energon\n3.Trend\n ");
				scanf("%d", &firstChoice);
				
				printf("Which feature would you like to modify second?\n\n");
				printf("1.Week\n2.Energon\n3.Trend\n ");
				scanf("%d", &secondChoice);
				
				if (firstChoice < 1 || firstChoice > 3 || secondChoice < 1 || secondChoice > 3)	{
					printf("Invalid choice. Please pick a number between 1 to 3.\n");
					printf("-----------------------------\n\n");
				}
				else if (firstChoice == secondChoice){
					printf("You have selected the same feature. Please choose different features.\n");
					printf("------------------------------\n\n");
				}
			} while (firstChoice < 1 || firstChoice > 3 || secondChoice < 1 || secondChoice > 3 || firstChoice == secondChoice);
	
			switch (firstChoice){
				case 1:
					do {
						printf("Please enter the week: (1 - %d)\n\n", MAX_WEEKS);
						scanf("%d", week);
						if (*week < 1 || *week > MAX_WEEKS)
							printf("Invalid number. Please input a number between 1 to 10\n");
					} while (*week < 1 || *week > MAX_WEEKS);
					break;
					
				case 2:
					do {
						printf("Please enter the amount of energon in storage: \n\n");
						scanf("%d", energonStorage);
						if (*energonStorage < 0)
							printf("Invalid number. Please input a positive number.\n");
					} while (*energonStorage < 0);
					break;
					
				case 3:
					do {
						printf("Please select the trend: \n");
						printf("0.) Scrap\n1.) Nominal\n2.) Primus\n\n");
						scanf("%d", devTrend);
						if (*devTrend < 0 || *devTrend > 2)
							printf("Invalid number. Please input a number between 0 to 2.\n");
					} while (*devTrend < 0 || *devTrend > 2);
					break;
			}
				
			switch (secondChoice) {
				case 1:
					do {
						printf("Please enter the week: (1 - %d)\n\n", MAX_WEEKS);
						scanf("%d", week);
						if (*week < 1 || *week > MAX_WEEKS)
							printf("Invalid number. Please input a number between 1 to 10\n");
					} while (*week < 1 || *week > MAX_WEEKS);
					break;
					
				case 2:
					do {
						printf("Please enter the amount of energon in storage: \n\n");
						scanf("%d", energonStorage);
						if (*energonStorage < 0)
							printf("Invalid number. Please input a positive number.\n");
					} while (*energonStorage < 0);
					break;
					
				case 3:
					do {
						printf("Please select the trend: \n");
						printf("0.) Scrap\n1.) Nominal\n2.) Primus\n\n");
						scanf("%d", devTrend);
						if (*devTrend < 0 || *devTrend > 2)
							printf("Invalid number. Please input a number between 0 to 2.\n");
					} while (*devTrend < 0 || *devTrend > 2);
					break;
			}
			break;
		}
		case 3: {
			
			do{
				printf("Please enter the week: (1 - %d)\n\n", MAX_WEEKS);
				scanf("%d", week);
				if (*week < 1 || *week > MAX_WEEKS)
					printf("Invalid number. Please input a number between 1 to 10.\n");
			} while (*week < 1 || *week > MAX_WEEKS);
				
			do{
				printf("Please enter the energon storage: \n\n");
				scanf("%d", energonStorage);
				if (*energonStorage < 0)
					printf("Invalid number. Please input a positive number.\n");
			} while (*energonStorage < 0);
			
			do {
				printf("Please select the trend: \n");
				printf("0.) Scrap\n1.) Nominal\n2.) Primus\n\n");
				scanf("%d", devTrend);
				if (*devTrend < 0 || *devTrend > 2)
					printf("Invalid number. Please input a number between 0 to 2.\n");
			} while (*devTrend < 0 || *devTrend > 2);
			break;
		}
		default:
			printf("Invalid Choice. \n");
			
	}
}

/***************************************************************************
------------------------------MAIN FUNCTION---------------------------------
***************************************************************************/

int main () {
	srand(time(NULL)); // seed for randomizer
	
	int week = 1, energonStorage = 10000, cubesInStorage = 0, stacksProduced = 0, stacksToSell = 0, devTrend = -1;
	int day, dailyPrice, currentTrend, costPerCube;
	int ifDevMode = 0; 
	
	// -------------------- Start of game --------------------------
	printf("-------------- ** ENERGON CUBES PRODUCTION !! ** ----------------- \n\n");
	printf("Pick one from the following options (1 or 2) for the game: \n");

	do {
		printf("1.) Developer's Mode\n2.) Normal Game\n");
		scanf("%d", &ifDevMode);
		if (ifDevMode < 1 || ifDevMode > 2)
			printf("Invalid number. Please pick 1 or 2. \n\n");
	} while (ifDevMode < 1 || ifDevMode > 2);
	
	
	if (ifDevMode == 1)
		devMode(&week, &energonStorage, &devTrend);
	
	while (week <= MAX_WEEKS) {
		printf("\nWeek %d   Day 1\n", week);
		printf("Energon storage: %d    Stacks: %d    \n", energonStorage, cubesInStorage / 10);
		
		costPerCube = generateEnergonCost();
		
		if (energonStorage >= 10 * costPerCube) {
			askStocks(costPerCube, &energonStorage, &stacksProduced);
			
			if (stacksProduced > 0)	{
				updateEnergonStorage(&energonStorage, -(stacksProduced * 10 * costPerCube));
				cubesInStorage += stacksProduced * 10;
			}
		
			// Start trend randomizer for the week
			if (devTrend != -1)
				currentTrend = devTrend;
			else
				currentTrend = randomTrend();
			
			// Start of Seliing
			for (day = 2; day <= 7; day++) {
				dailyPrice = getDailyPrice(costPerCube, currentTrend);
				
				printf("-------------------------------\n\n");
				printf("Week %d   Day %d\n", week, day);
				printf("Energon Storage: %d		Stacks: %d\n\n", energonStorage, cubesInStorage / 10);
				
				printf("Swindle is buying Energon Cubes for %d Energon per cube\n", dailyPrice);
				printf("You can earn %d Energon per stack.\n\n", dailyPrice * 10);
			
				if (cubesInStorage > 0) {
					do {
						printf("How many stacks do you wish to sell to Swindle? ");
						scanf("%d", &stacksToSell);
						
						if (stacksToSell < 0)
							printf("Invalid input! Please put a positive number or 0.\n\n");
						else if (stacksToSell * 10 > cubesInStorage)
							printf("Not enough stacks!\n\n");
					} while (stacksToSell < 0 || stacksToSell * 10 > cubesInStorage);
					
					cubesInStorage -= stacksToSell * 10;
					updateEnergonStorage(&energonStorage, stacksToSell * 10 * dailyPrice);
					
					if (stacksToSell > 0) {
						printf("%d stacks sold. \n", stacksToSell);
						printf("You earned %d Energon.\n\n", stacksToSell * 10 * dailyPrice);
					} else
						printf("No stacks sold today.\n\n");
				} else
					printf("No cubes left to sell today!\n\n");
			}
			// Reset cubes by the end of the week
			cubesInStorage = 0;
		} else if (energonStorage < 800 || costPerCube * 10 < 800) {
			printf("Not enough energon to produce one cube!! Energon storage is below 800!\n\n");
			week = MAX_WEEKS + 1;
		} else {
			printf("Not enough energon to produce one stack currently. Skipping to next week.\n\n");
		}
		
		if (week <= MAX_WEEKS) {
			week++;
		}
	}
	
	// ---------- Game Over ---------------
	printf("Game Over! Energon Storage: %d\n", energonStorage);
	
	if(energonStorage >= TARGET_ENERGON) {
		printf("------------------------------------------------------------\n");
		printf("----------* * You achieved the goal of 1 million Energon! Yay! * * --------------\n\n");
		printf("------------------------------------------------------------\n");
	}
	else
		printf("You didn't reach the goal of 1 million energon. Better luck next time!\n\n");
	
	return 0;
}	
