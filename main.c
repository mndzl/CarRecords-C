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
    * Size from user - 
    * Create array of pointers - 
    * Function to create a node - 
    * Menu - DONE
    * Function to add an auto - 
    * Function display output by cost low to high - s
    * Function display output by Model A to Z - 
    * Function to write all data to a binary file - 
    * Functiuon to free all memory from the heap - 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    char make[50];
    char model[50];
    int yearBuilt;
    float cost;

} AUTO;

// Displays menu and returns option from user
void displayMenu(char option[]);

void addAuto(AUTO**, int*, int);

void displayCostLowToHigh(AUTO**);

void displayMakeDescendingOrder(AUTO**);

int retrieveData(AUTO***, int*, int*);

int createFile(AUTO***, int*, int*);

void exitProgram(AUTO**);

int main(){
    // Program variables
    char option[3];
    AUTO** autos = NULL;
    int eSize = 0;
    int maxCars = 0;

    // Retrieve data from file or create new record list
    int data = retrieveData(&autos, &eSize, &maxCars);
    if (!data) return -1;
    printf("%i cars added. Limit: %i\n", eSize, maxCars);
    printf("Car 1 is a %s.\n", autos[0]->model);

    // Retrieve menu option
    displayMenu(option);
    printf("You selected %s\n", option);
    // Single letter
    if(option[0] == 'Q') exitProgram(autos);
    else if(option[0] == 'A') {addAuto(autos, &eSize, maxCars); printf("Car %s created.\n", autos[eSize-1]->model);}
    else if(option[0] == 'D'){
        // Double letter
        if(option[1] == 'C') displayCostLowToHigh(autos);
        else if(option[1] == 'M') displayMakeDescendingOrder(autos);
    }
    return 0;

}

void exitProgram(AUTO** autos){
    printf("Exiting program.\n");
    free(autos);
    exit(1);
}

void displayMenu(char finalOption[]){
    char option[3];
    printf("------- MAIN MENU -------\n");
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
                printf("Invalid option, please try again.\n");
            }
            else valid = 1;
            // printf("Single letter!\n");

        }else{
            option[0] = toupper(option[0]);
            option[1] = toupper(option[1]);

            // printf("0: %c, 1: %c\n", option[0], option[1]);
            // It is display, checking order...
            if (option[0] != 'D' || (option[1] != 'C' && option[1] != 'M')) {
                printf("\nInvalid option, please try again.\n");
            }
            else valid = 1;
            // printf("Double letter!\n");
        }
    }

    strcpy(finalOption, option);
}

int retrieveData(AUTO*** autos, int* eSize, int* maxCars){
    int exitCode;

    exitCode = createFile(autos, eSize, maxCars);

    if (exitCode == 1) printf("File created succesfully.\n");
    else printf("There was an error creating the file.\n");

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

    strcpy((*autos)[0]->model, "Hyundai");

    return autos != NULL;
}

void addAuto(AUTO** autos, int* eSize, int maxCars){
    printf("eSize is %i\n", *eSize);
    if(*eSize == maxCars){
        printf("Could not create. You have reached the maximum number of cars (%i).\n", maxCars);
        return;
    }

    printf("======== ADDING CAR ========\n");

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
    
    printf("Car created.\n");
}

void displayCostLowToHigh(AUTO** autos){}

void displayMakeDescendingOrder(AUTO** autos){}