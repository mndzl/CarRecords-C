#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char make[50];
    char model[50];
    int yearBuilt;
    float cost;

} AUTO;

int main(){
    AUTO** orderedlist;

    int swapped;
    do{
        // flag
        swapped = 0;

        for(int i=0; i<eSize-1; i++){
            for(int j=i+1; j<eSize; j++){
                if(autos[i]->cost > autos[j]->cost){
                    AUTO dummy;
                    memcpy(dummy, autos[i]);
                    
                }
            }
        }

    }while(swapped)    return 0;
}