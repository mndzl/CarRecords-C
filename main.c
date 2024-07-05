/*
    Written by Luis Mendez
    Date Written: May 24, 2023
    Purpose: 
        Disjoin-Auto-Assignment
        Create a program to store information about automobiles. 

    ------- CAR ATTRIBUTES -------
    * make
    * model
    * yearBuilt
    * cost
    
    ------ REQUIREMENTS ------
    * Utilize menu system with:
        * [A]dd one automobile
        * [D]isplay all automobiles by
        *   [C]ost low to high
        *   [M]ake descending order
        * [Q]uit program
    
    * Recover data file of automobile information if available. Begin program where user left of
    
    * If no previus data file, program will ask how many records will be needed.
    
    * Memory should be released from the heap

    * Perform error checking when saving to file or heap

    ------- TODO -------
    * Size from user - DONE
    * Create array of pointers - DONE
    * Function to create a node - DONE
    * Menu - DONE
    * Function to add an auto - DONE
    * Function display output by cost low to high - DONE
    * Function display output by Make A to Z - DONE
    * Function to write all data to a binary file - 
    * Function to free all memory from the heap - 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define PAUSE myPause()

typedef struct{
    char make[50];
    char model[50];
    int yearBuilt;
    float cost;

} AUTO;

// Displays menu and returns option from user
void displayMenu(char option[]);

void addAuto(AUTO**, int*, int);

void displayCostLowToHigh(AUTO**, int);

void displayMakeAscendingOrder(AUTO**, int);

int retrieveData(AUTO***, int*, int*);

int createFile(AUTO***, int*, int*);

void exitProgram(AUTO**, int);

// Coloring
void purple(){printf("\033[1;35m");};
void blue(){printf("\033[1;34m");}
void green(){ printf("\033[1;32m"); }
void red () { printf("\033[1;31m"); }
void yellow (){ printf("\033[1;33m"); }
void reset () { printf("\033[0m"); }

void myPause() {
	getchar();
	printf("\nPress ENTER to continue....\n");
	getchar();
}

int main(){
    // Program variables
    char option[3];
    AUTO** autos = NULL;
    int eSize = 0;
    int maxCars = 0;

    // Retrieve data from file or create new record list
    int data = retrieveData(&autos, &eSize, &maxCars);
    if (!data) return -1;
    // printf("%i cars added. Limit: %i\n", eSize, maxCars);
    // printf("Car 1 is a %s.\n", autos[0]->model);

    while(1){
        // Retrieve menu option
        displayMenu(option);
        // printf("You selected %s\n", option);
        // Single letter
        if(option[0] == 'Q') exitProgram(autos, eSize);
        else if(option[0] == 'A') addAuto(autos, &eSize, maxCars);
        else if(option[0] == 'D'){
            // Double letter
            if(option[1] == 'C') displayCostLowToHigh(autos, eSize);
            else if(option[1] == 'M') displayMakeAscendingOrder(autos, eSize);
        }
    }

    return 0;
}

void exitProgram(AUTO** autos, int eSize){
    printf("\nExiting...\n");
    for(int i=0; i<eSize; i++) free(autos[i]);
    free(autos);
    exit(1);
}

void displayMenu(char option[]){
    printf("\n------- MAIN MENU -------\n");
    printf("[A]dd one automobile\n");
    printf("[D]isplay all automobiles by\n");
    printf("\t[C]ost low to high\n");
    printf("\t[M]ake ascending order\n");
    printf("[Q]uit program\n");


    int valid = 0;
    while (!valid){
        printf("Option: "); scanf(" %2s", option);

        // Error handling
        if (option[1] == '\0'){
            option[0] = toupper(option[0]);

            // It is either Add or Quit
            if (option[0] != 'A' && option[0] != 'Q'){
                yellow();
                printf("Invalid option, please try again.\n");
                reset();
            }
            else valid = 1;
            // printf("Single letter!\n");

        }else{
            option[0] = toupper(option[0]);
            option[1] = toupper(option[1]);

            // printf("0: %c, 1: %c\n", option[0], option[1]);
            // It is display, checking order...
            if (option[0] != 'D' || (option[1] != 'C' && option[1] != 'M')) {
                yellow();
                printf("\nInvalid option, please try again.\n");
                reset();
            }
            else valid = 1;
            // printf("Double letter!\n");
        }
    }
}

int retrieveData(AUTO*** autos, int* eSize, int* maxCars){
    int exitCode;

    exitCode = createFile(autos, eSize, maxCars);

    if (exitCode == 1){
        green();
        printf("File created succesfully.\n");
        reset();
    }
    else {
        red();
        printf("There was an error creating the file.\n");
        reset();
    }

    return exitCode;
}

int createFile(AUTO*** autos, int* eSize, int* n){
    printf("Enter number of cars to add: "); scanf(" %i", n);

    *autos = calloc(*n, sizeof(AUTO*));
    if (*autos == NULL) return 0;
    for(int i=0; i<*n; i++){
        (*autos)[i] = calloc(1, sizeof(AUTO));
        if((*autos)[i] == NULL) return 0;
    }

    return autos != NULL;
}

void addAuto(AUTO** autos, int* eSize, int maxCars){
    // printf("eSize is %i\n", *eSize);
    if(*eSize == maxCars){
        red();
        printf("Could not create. You have reached the maximum number of cars (%i).\n", maxCars);
        reset();
        PAUSE;
        return;
    }

    printf("\n======== ADDING CAR ========\n");

    char make[50];
    char model[50];
    int yearBuilt;
    float cost;

    // Make
    printf("Please enter car make: "); scanf(" %s", make);

    // Model
    printf("Please enter car model: "); scanf(" %s", model);
    
    // YearBuilt
    printf("Please enter car year: ");
    while(scanf(" %d", &yearBuilt) == 0){
	    printf("Invalid numeric value.. try again: ");
        while(getchar() !='\n');
    }   

    // CarCost
    printf("Please enter car cost: ");
    if(scanf(" %f", &cost) == 0){
	    printf("Invalid numeric value.. try again: ");
        while(getchar() !='\n');
    }

    // Saving new car
    strcpy(autos[*eSize]->make, make);
    strcpy(autos[*eSize]->model, model);
    autos[*eSize]->yearBuilt = yearBuilt;
    autos[*eSize]->cost = cost;
    (*eSize)++;
    
    green();
    printf("Car created.\n");
    reset();
    PAUSE;
}

void displayCostLowToHigh(AUTO** autos, int eSize){
    if (eSize == 0){
        yellow();
        printf("No autos added to system.\n");
        reset();
        return;
    }

    // Sorting by cost from low to high
    int swapped = 0;
    do{
        // flag
        swapped = 0;

        for(int i=0; i<eSize-1; i++){
            for(int j=i+1; j<eSize; j++){
                if(autos[i]->cost > autos[j]->cost){
                    AUTO dummy;
                    dummy = *autos[i];
                    *autos[i] = *autos[j];
                    *autos[j] = dummy;
                    
                    swapped = 1;
                    printf("SWAP!");
                }
            }
        }

    }while(swapped);

    // Printing
    printf("\n======= CARS BY COST (LOW TO HIGH) =======\n");
    printf("MAKE\tMODEL\tYEAR\tCOST\n");

    for(int i=0; i<eSize; i++){
        printf("%s\t%s\t%i\t%.2f\n", autos[i]->make, autos[i]->model, autos[i]->yearBuilt, autos[i]->cost);
    }
    PAUSE;

}

void displayMakeAscendingOrder(AUTO** autos, int eSize){
        if (eSize == 0){
        yellow();
        printf("No autos added to system.\n");
        reset();
        return;
    }

    // Sorting by make ascending
    int swapped = 0;
    do{
        // flag
        swapped = 0;

        for(int i=0; i<eSize-1; i++){
            for(int j=i+1; j<eSize; j++){
                if(strcmp(autos[i]->make, autos[j]->make) > 0){
                    AUTO* dummy;
                    dummy = autos[i];
                    autos[i] = autos[j];
                    autos[j] = dummy;
                    
                    swapped = 1;
                }
            }
        }

    }while(swapped);

    // Printing
    printf("\n======= CARS BY MAKE (ASCENDING) =======\n");
    printf("MAKE\tMODEL\tYEAR\tCOST\n");

    for(int i=0; i<eSize; i++){
        printf("%s\t%s\t%i\t%.2f\n", autos[i]->make, autos[i]->model, autos[i]->yearBuilt, autos[i]->cost);
    }
    PAUSE;
}