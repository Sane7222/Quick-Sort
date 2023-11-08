/*
 *      Matias Moseley     CS450     10/7/2022     Sort 1 million players XP
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define ARR_LEN 1000000
#define LINE_LEN 5
#define NUM_SKILLS 5
#define NUM_LISTS 6
#define RANGE 10001
#define TWOPERCENT 200000

int compare(const void *a, const void *b);
void MySort(int bigArr[], int low, int lastIndex);

void main(int argc, char *argv[]){

        int n, p, a, b, c, d, e, a1 = 0, b1 = 0, c1 = 0, d1 = 0, e1 = 0;
        int *skill_breakdancing = (int*)(malloc(ARR_LEN*sizeof(int))), *skill_apiculture = (int*)(malloc(ARR_LEN*sizeof(int))), *skill_basket = (int*)(malloc(ARR_LEN*sizeof(int)));
        int *skill_xbasket = (int*)(malloc(ARR_LEN*sizeof(int))), *skill_sword = (int*)(malloc(ARR_LEN*sizeof(int))), *total_xp = (int*)(malloc(ARR_LEN*sizeof(int)));
        int *skills[NUM_LISTS];
        char *names[NUM_LISTS];
        float time_sum = 0;

        for(int i = 0; i < LINE_LEN*ARR_LEN; i++){ // Iterate through exactly 1 million players | Change ARR_LEN to number of players
                fscanf(stdin, "%d", &n);
                p = i % NUM_SKILLS;
                switch(p){ // Add correct stdin element to correct xp array
                        case 0:
                                a = n;
                                skill_breakdancing[a1++] = n;
                                break;
                        case 1:
                                b = n;
                                skill_apiculture[b1++] = n;
                                break;
                        case 2:
                                c = n;
                                skill_basket[c1++] = n;
                                break;
                        case 3:
                                d = n;
                                skill_xbasket[d1++] = n;
                                break;
                        case 4:
                                e = n;
                                skill_sword[e1] = n;
                                int sum = a+b+c+d+e;
                                total_xp[e1++] = sum;
                                break;
                        default:
                                fprintf(stderr, "Error reading from STDIN\n");
                                return;
                }
        }

        skills[0] = skill_breakdancing; // Set repective arrays
        skills[1] = skill_apiculture;
        skills[2] = skill_basket;

        skills[3] = skill_xbasket;
        skills[4] = skill_sword;
        skills[5] = total_xp;

        names[0] = "SKILL_BREAKDANCING\0"; // Set array names
        names[1] = "SKILL_APICULTURE\0";
        names[2] = "SKILL_BASKET\0";
        names[3] = "SKILL_XBASKET\0";
        names[4] = "SKILL_SWORD\0";
        names[5] = "TOTAL_XP\0";

        if(!strcmp(argv[1], "standard")){ // Standard called
                for(int i = 0; i < NUM_LISTS; i++){
                        clock_t start = clock();

                        qsort(skills[i], ARR_LEN, sizeof(int), compare); // Standard library quicksort

                        double time_taken_in_seconds = (double)( clock() - start ) / CLOCKS_PER_SEC;
                        double time_taken_in_microseconds = time_taken_in_seconds * 1000000.0;
                        time_sum += time_taken_in_microseconds;

                        fprintf(stdout, "%s\n", names[i]); // Print array to STDOUT
                        for(int j = 0; j < ARR_LEN; j++){
                                fprintf(stdout, "%d\n", skills[i][j]);
                        }

                        fprintf(stdout, "time taken: %f\n\n", time_taken_in_microseconds);
                        free(skills[i]);
                }
                fprintf(stdout, "total time taken: %f\n", time_sum);
        }

        else if(!strcmp(argv[1], "custom")){ // Custom called
                for(int i = 0; i < NUM_LISTS; i++){

                        int *big = (int*)(malloc(TWOPERCENT * sizeof(int))), *countOut = (int*)(malloc(ARR_LEN*sizeof(int))); // Allocate memory | Twopercent is actually 4% of 1 million
                        int *count = (int*)(calloc(RANGE, sizeof(int)));
                        int bigPos = 0, countPos = 0;

                        clock_t start = clock();

                        for(int j = 0; j < ARR_LEN; j++){ // Seperate skill array into high xp and low xp
                                if(skills[i][j] < RANGE) { count[skills[i][j]]++; countOut[countPos++] = skills[i][j]; }
                                else { big[bigPos++] = skills[i][j]; }
                        }
                        int *out = (int*)(malloc(countPos*sizeof(int)));

                        for(int j = 1; j < RANGE; j++){ // Set correct position of low xp in the output array
                                count[j] += count[j-1];
                        }

                        for(int j = countPos-1; j >= 0; j--){ // Create output array
                                int x = countOut[j];
                                int y = count[x];
                                y--;
                                out[y] = countOut[j]; // Use countOut's count-1 position for the output array
                                count[countOut[j]]--;
                        }

                        MySort(big, 0, bigPos-1); // My implementation of quicksort

                        double time_taken_in_seconds = (double)( clock() - start ) / CLOCKS_PER_SEC;
                        double time_taken_in_microseconds = time_taken_in_seconds * 1000000.0;
                        time_sum += time_taken_in_microseconds;

                        fprintf(stdout, "%s\n", names[i]); // Print array to STOUT

                        for(int j = bigPos-1; j > -1; j--){
                                fprintf(stdout, "%d\n", big[j]);
                        }
                        for(int j = countPos-1; j > -1; j--){
                                fprintf(stdout, "%d\n", out[j]);
                        }

                        fprintf(stdout, "time taken: %f\n\n", time_taken_in_microseconds);
                        free(big); free(skills[i]); free(countOut); free(out); free(count);
                }
                fprintf(stdout, "total time taken: %f\n", time_sum);
        }
}

int compare(const void *a, const void *b){ // Standard lib qsort uses compare function
        int data1 = *(int *)a, data2 = *(int *)b;
        if(data1 > data2) return -1;
        if(data1 < data2) return 1;
        return 0;
}

void MySort(int bigArr[], int low, int lastIndex){ // My implementation of quicksort
        if(low < lastIndex){
                int temp;
                int pivot = bigArr[lastIndex]; // Pivot is last item in array
                int i = (low-1);

                for(int j = low; j <= lastIndex - 1; j++){ // Iterate through array
                        if(bigArr[j] < pivot){
                                i++;
                                temp = bigArr[i]; // Swap elements
                                bigArr[i] = bigArr[j];
                                bigArr[j] = temp;
                        }
                }
                temp = bigArr[i+1]; // Swap elements
                bigArr[i+1] = bigArr[lastIndex];
                bigArr[lastIndex] = temp;

                pivot = i+1; // Set new pivot

                MySort(bigArr, low, pivot-1); // Quicksort the lower and higher half of pivot
                MySort(bigArr, pivot+1, lastIndex);
        }
}