// Devoir S1825 Code C

//--------- Exercice 2 ---------
// Version vectorielle

#include <stdio.h>
#include <stdlib.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#include <sys/time.h>



int vect_find(int *U, int i_start, int i_end, int i_step, int val, int **ind_val) {

    __m128 U_onRegistre ;
    __m128 Val ;
    __m128 temp;
    float v[4] __attribute__((aligned(64)));
    int numberOccurrence = 0;
    int last_i;
    Val = _mm_set_ps(val, val, val, val);

    for (int i = i_start; i < i_end + 1; i = i + 4 * i_step) {
        //printf("%d\n", i );
        U_onRegistre = _mm_set_ps(U[i], U[i + i_step], U[i + 2 * i_step], U[i + 3 * i_step]);
        temp = _mm_cmpeq_ps(U_onRegistre, Val);
        _mm_store_ps(v, temp);
        for (int j = 0; j < 4; j++) {

            if ((int) v[j] != 0) {
                numberOccurrence++;
                (*ind_val) = realloc((*ind_val), (numberOccurrence) * sizeof(int));
                //
                (*ind_val)[numberOccurrence - 1] = i + (3 - j) * i_step;
            }
        }
        last_i = i;
    }

    for (int i = last_i + i_step; i < i_end + 1;  i = i + i_step)
    {
        if (U[i] == val)
        {
            numberOccurrence++;
            (*ind_val) = realloc((*ind_val), (numberOccurrence) * sizeof(int));
            (*ind_val)[numberOccurrence - 1] = i;
        }
    }

    return numberOccurrence;
}


int main(int argc, char *argv[]) {



    int N =  50;
    int val = 9;
    int i_step = 2;
    int i_start = 0;
    int i_end = 40;

    int number;

    int **ind_val = malloc(sizeof(int**));
    *ind_val = malloc(sizeof(int*));

    int U[N];



    printf("Exemple d'application \n");
    printf("Vecteur U : \n");
    for (int i = 0; i < N; ++i)
    {
        if (i % 10 == 0) {
            printf("\n");
        }
        U[i] = rand() % 50;
        printf("%d ", U[i]);

    }


    printf("\n\nExemple avec i_start : %d \n", i_start );
    printf("Exemple avec i_end : %d \n", i_end );
    printf("Exemple avec i_step : %d \n", i_step );
    printf("Exemple avec val : %d \n", val );


    number =  vect_find(U, i_start, i_end, i_step, val, ind_val);

    for (int j = 0; j < number ; ++j)
    {
        printf("\n -- Position final -- %d ", ((int *) *ind_val)[j] );
    }

    return 0;
}
////////////////////////////////////////////////////

