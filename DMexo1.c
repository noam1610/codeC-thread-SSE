// Devoir S1825 Code C

//--------- Exercice 1 ---------

#include <stdio.h>
#include <stdlib.h>

/* 
----------Réponse aux Questions----------

Question1: 
 
  void *realloc(void *ptr, size_t size)

  Si la taille size est superieur à la taille déjà alouée à ptr alors, l'espace mémoire occupé
  par ptr est recopié en mémoire et complété jusqu'à ce que la mémoire alouée ait la taille size.
  Le pointeur associé pointe donc vers une autre addresse en mémoire.

  Si size est null alors ptr n'est plus aloué en mémoire et passe à null.


Question 2:
  
  Pour conserver en mémoire uniquement les indices nécessaires, il est nécessaire de réalouer de l'espace dynamiquement.
  Ainsi
  La fonction realloc modifie la position du tableau en mémoire. Ainsi le pointeur relatif à ce tableau change au fur et à mesure.
  Enfin, comme toutes les opératiions effectuées se font sur des variables locales, les modifications ne seront pas effectives dans le main.
  Il faut donc passé un pointeur en paramètre qui pointe vers le tableau c'est à dire un pointeur de pointeur.


Question 3:

  Pour passer sur les indices pairs on prend simplement le premier element de U pour i_start soit 0 et i_end = N et i_step = 2

*/


int find(int *U, int i_start, int i_end, int i_step, int val, int **ind_val){

    int numberOccurrence = 0;
   
    for (int i = i_start; i < i_end +1; i = i + i_step){
        if(U[i] == val){
          numberOccurrence++;
          (*ind_val) = realloc((*ind_val), (numberOccurrence)*sizeof(int));
          (*ind_val)[numberOccurrence - 1] = i;    
        }
    }  
   
    return numberOccurrence;
}


int main(int argc, char *argv[]){

    int N =  50;
    int val = 29;
    int i_step = 1;
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
        if(i%10 == 0){
          printf("\n");
        }
        U[i] = rand() % 50;
        printf("%d ", U[i]);

    }

 
  printf("\n\nExemple avec i_start : %d \n", i_start );
  printf("Exemple avec i_end : %d \n", i_end );
  printf("Exemple avec i_step : %d \n", i_step );
  printf("Exemple avec val : %d \n", val );


    number =  find(U, i_start, i_end, i_step, val, ind_val);
  
  for (int j = 0; j < number ; ++j)
    {
        printf("\n -- Position final -- %d ", ((int *) *ind_val)[j] );
    }

    return 0;
}
////////////////////////////////////////////////////

