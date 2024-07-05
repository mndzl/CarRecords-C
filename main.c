/*
    Written by Luis Mendez
    Date Written: Jul 5, 2024
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
    * Function to write all data to a binary file - DONE
    * Function to free all memory from the heap - DONE
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

// Adds auto to record and saves to file
void addAuto(AUTO**, int*, int);

// Creates file
int createFile(AUTO***, int*, int*);

// Displays all cars ordered by cost from low to high
void displayCostLowToHigh(AUTO**, int);

// Displays menu and returns option from user
void displayMenu(char option[]);

// Displays all cars ordered by make from A to Z
void displayMakeAscendingOrder(AUTO**, int);

// Exits program and frees the auto array
void exitProgram(AUTO**, int, int);

// Helper function for pausing
void myPause();

// Makes effective changes into file
void saveChanges(AUTO**, int, int);

// Retrieves data from file or creates is if it doesnt exist
int retrieveData(AUTO***, int*, int*);

// Coloring
void purple(){printf("\033[1;35m");};
void blue(){printf("\033[1;34m");}
void green(){ printf("\033[1;32m"); }
void red () { printf("\033[1;31m"); }
void yellow (){ printf("\033[1;33m"); }
void reset () { printf("\033[0m"); }


int main(){
    // Program variables
    char option[3];
    AUTO** autos = NULL;
    int eSize = 0;
    int maxCars = 0;

    // Retrieve data from file or create new record list
    int data = retrieveData(&autos, &eSize, &maxCars);

    // Main flow
    while(1){
        // Retrieve menu option
        displayMenu(option);
        // printf("You selected %s\n", option);
        // Single letter
        if(option[0] == 'Q') exitProgram(autos, eSize, maxCars);
        else if(option[0] == 'A') addAuto(autos, &eSize, maxCars);
        else if(option[0] == 'D'){
            // Double letter
            if(option[1] == 'C') displayCostLowToHigh(autos, eSize);
            else if(option[1] == 'M') displayMakeAscendingOrder(autos, eSize);
        }
    }

    return 0;
}

void exitProgram(AUTO** autos, int eSize, int maxCars){
    // Saving changes into file
    saveChanges(autos, eSize, maxCars);
    printf("\nExiting...\n");

    // Freeing memory
    for(int i=0; i<eSize; i++) free(autos[i]);
    free(autos);

    exit(1);
}

void saveChanges(AUTO** autos, int eSize, int maxCars){
    yellow();
    printf("\nUpdating file... ");
    reset();

    // Write values on file
    FILE* f = fopen("autorecords.bin", "wb");
    if(f == NULL){
        red();
        printf("Error.");
        reset();

        return;
    }
    fwrite(&eSize, sizeof(int), 1, f);
    fwrite(&maxCars, sizeof(int), 1, f);
    for(int i=0; i<eSize; i++){
        fwrite(autos[i], sizeof(AUTO), 1, f);
    }

    green();
    printf("Done.\n");
    fclose(f);
    reset();
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

        }else{
            option[0] = toupper(option[0]);
            option[1] = toupper(option[1]);

            // It is display, checking order...
            if (option[0] != 'D' || (option[1] != 'C' && option[1] != 'M')) {
                yellow();
                printf("\nInvalid option, please try again.\n");
                reset();
            }
            else valid = 1;
        }
    }
}

int retrieveData(AUTO*** autos, int* eSize, int* maxCars){
    int exitCode = 0;
    FILE* f = fopen("autorecords.bin", "rb");
    if(f == NULL){
        // No file found, creating one
        createFile(autos, eSize, maxCars);
    }else{
        // Reading from file and allocating memory for auto array
        fread(eSize, sizeof(int), 1, f);
        fread(maxCars, sizeof(int), 1, f);

        *autos = calloc(*maxCars, sizeof(AUTO*));
        if (*autos == NULL) return 0;
        for(int i=0; i<*maxCars; i++){
            (*autos)[i] = calloc(1, sizeof(AUTO));
            if((*autos)[i] == NULL) return 0;
            fread((*autos)[i], sizeof(AUTO), 1, f);
        }

        if(*autos != NULL){
            green();
                printf("File recovered succesfully.\n");
            reset();
            exitCode = 1;
        }else{
            red();
                printf("There was an error recovering the file.\n");
            reset(); 
                exitCode = 0;
        }
    }

    fclose(f);

    return exitCode;
}

int createFile(AUTO*** autos, int* eSize, int* n){
    yellow();
    printf("Creating file... ");
    FILE* f = fopen("autorecords.bin", "wb");
    if(f == NULL){
        red();
        printf("Error.");
        reset();

        return 0;
    }else{
        green();
        printf("Done.\n");
        reset();
    }
    reset();

    // Allocating space for newly created file and auto array
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
    // Check if still space for more cars
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
    saveChanges(autos, *eSize, maxCars);
    PAUSE;
}

void displayCostLowToHigh(AUTO** autos, int eSize){
    if (eSize == 0){
        yellow();
        printf("No autos added to system.\n");
        reset();
        return;
    }

    // Sorting by cost from low to high with bubble sort
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

    // Sorting by make ascending with bubble sort
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

void myPause() {
	getchar();
	printf("\nPress ENTER to continue....\n");
	getchar();
}