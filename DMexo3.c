// Devoir S1825 Code C

//--------- Exercice 3 ---------
// Version avec thread
/*

  ----Question a---- 

La structure d'une thread est la suivante:

  int pthread_create(pthread_t * thread, pthread_attr_t * attr,
       void *(*start_routine) (void *), void *arg);

La fonction renvoie  0 si la création a été réussie ou une autre valeur si il y a eu une erreur.

Le premier argument est un pointeur vers l'identifiant du thread.

Le second argument désigne les attributs du thread.

Le troisième argument est un pointeur vers la fonction à exécuter dans le thread.


  ----Question b----


*/

#include <stdio.h>
#include <stdlib.h>
#include <xmmintrin.h> 
#include <emmintrin.h> 
#include <pmmintrin.h>
#include <sys/time.h>
#include <pthread.h>


int numberOccurrenceTotal;
int numberOccurrenceMaximal;
pthread_mutex_t number_mutex = PTHREAD_MUTEX_INITIALIZER;

struct struct_data {
      int *U;
      int i_start;
      int i_end;
      int i_step;
      int val;
      int **ind_val;
    };



int find(int *U, int i_start, int i_end, int i_step, int val, int **ind_val){

     for (int i = i_start; i < i_end +1; i = i + i_step){

        if(numberOccurrenceTotal >= numberOccurrenceMaximal){
            pthread_exit(NULL);

        }
        if(U[i] == val){

          pthread_mutex_lock(&number_mutex);
          
          numberOccurrenceTotal++;

          (*ind_val) = realloc((*ind_val), (numberOccurrenceTotal)*sizeof(int));

          (*ind_val)[numberOccurrenceTotal - 1] = i;
          
          pthread_mutex_unlock(&number_mutex);

        }
    }  
    return numberOccurrenceTotal;
}

void* sub_find(void *args){

  int *U;
  int i_start;
  int i_end;
  int i_step;
  int val;
  int **ind_val;

  struct struct_data *thread_args;
  thread_args = (struct struct_data*) args;
  
  U = thread_args->U;
  i_start = thread_args -> i_start;
  i_end = thread_args -> i_end;
  i_step = thread_args -> i_step;
  val = thread_args -> val;
  ind_val = thread_args -> ind_val;

  find(U,i_start,i_end,i_step,val,ind_val);
  
};

int vect_find(int *U, int i_start, int i_end, int i_step, int val, int **ind_val){

    __m128 U_onRegistre ;
    __m128 Val ;
    __m128 temp;
    float v[4] __attribute__((aligned(64)));
    int numberOccurrence = 0;
    int last_i;
    Val = _mm_set_ps(val, val, val, val);

    for (int i = i_start; i < i_end +1; i = i + 4*i_step){


        U_onRegistre = _mm_set_ps(U[i],U[i+i_step],U[i+2 * i_step],U[i + 3*i_step]);
        temp = _mm_cmpeq_ps(U_onRegistre, Val);
        _mm_store_ps(v, temp);
        for(int j=0; j<4; j++){
          if(numberOccurrenceTotal >= numberOccurrenceMaximal){
            pthread_exit(NULL);
          }
          if((int) v[j] != 0){
             pthread_mutex_lock(&number_mutex);
                numberOccurrence++;
                (*ind_val) = realloc((*ind_val), (numberOccurrence)*sizeof(int));
                (*ind_val)[numberOccurrence - 1] = i + (3-j)*i_step;
             pthread_mutex_unlock(&number_mutex);
          }
        }
        last_i = i;
    }

    for (int i = last_i + i_step; i < i_end+1;  i = i + i_step)
    {
         if(numberOccurrenceTotal >= numberOccurrenceMaximal){
            pthread_exit(NULL);
          }
        if(U[i] == val)
        {
                  numberOccurrence++;
                  (*ind_val) = realloc((*ind_val), (numberOccurrence)*sizeof(int));
                  (*ind_val)[numberOccurrence - 1] = i;    
        }
    }

    return numberOccurrence;
}

void* sub_vect_find(void *args){

  int *U;
  int i_start;
  int i_end;
  int i_step;
  int val;
  int **ind_val;

  struct struct_data *thread_args;
  thread_args = (struct struct_data*) args;
  
  U = thread_args->U;
  i_start = thread_args -> i_start;
  i_end = thread_args -> i_end;
  i_step = thread_args -> i_step;
  val = thread_args -> val;
  ind_val = thread_args -> ind_val;

  vect_find(U,i_start,i_end,i_step,val,ind_val);
  
};

int thread_find(int *U,int i_start, int i_end, int i_step, int val, int **ind_val, int k, int ver ){
  

  numberOccurrenceTotal = 0;
  int numberForThread = (i_end - i_start)/(4);
  numberOccurrenceMaximal = k;
  printf("indices par thread : %d \n", numberForThread);

  pthread_t table_thread[4];
  struct struct_data data[4];
  
  data[0].U = data[1].U =  data[2].U =  data[3].U = U;
  data[0].val = data[1].val =  data[2].val =  data[3].val = val;
  data[0].ind_val = data[1].ind_val =  data[2].ind_val =  data[3].ind_val = ind_val;
  
  for (int i = 0; i < 4; ++i)
  {
    data[i].i_start = i_start + i * numberForThread *i_step ;
    data[i].i_step = i_step;
    data[i].i_end =  i_start + (1+i) * numberForThread *i_step - 1;
  }
  
  data[4].i_end = i_end;


if(ver == 0){
  printf("Creation des threads sans SIMD\n");
    for (int i = 0; i < 4; ++i)
      {
        if (pthread_create(&table_thread[i], NULL, sub_find, &data[i])) {
              perror("pthread_create");
              return EXIT_FAILURE;
          }
      }
}

if(ver == 1){
  printf("Creation des threads avec SIMD \n");
    for (int i = 0; i < 4; ++i)
      {
        if (pthread_create(&table_thread[i], NULL, sub_vect_find, &data[i])) {
              perror("pthread_create");
              return EXIT_FAILURE;
          }
      }
}

      for (int i = 0; i < 4; ++i)
      {
        if (pthread_join(table_thread[i], NULL)) {
              perror("pthread_join");
              return EXIT_FAILURE;
          }
      }
    printf("Fin des threads \n");

    printf(" numberOccurrenceTotal : %d \n", numberOccurrenceTotal );
  



  return numberOccurrenceTotal;
}



int main(int argc, char *argv[]){


    int N =  40;
    int val = 2;
    int i_step = 1;
    int i_start = 0;
    int i_end = 40;
    
    int U[N];
   
    int **ind_val = malloc(sizeof(int**));
    *ind_val = malloc(sizeof(int*));

    numberOccurrenceMaximal = 4;
    numberOccurrenceTotal = 0;

    printf("Exemple d'application \n");
    printf("Vecteur U : \n");
    for (int i = 0; i < N; ++i)
    {
       if(i%10 == 0){
          printf("\n");
        }
        U[i] = rand() % 10;
        printf("%d ", U[i]);
    }

 
  printf("\n\nExemple avec i_start : %d \n", i_start );
  printf("Exemple avec i_end : %d \n", i_end );
  printf("Exemple avec i_step : %d \n", i_step );
  printf("Exemple avec val : %d \n", val );


  numberOccurrenceTotal =  thread_find(U, i_start, i_end, i_step, val, ind_val, 4, 0);
  
  for (int j = 0; j < numberOccurrenceTotal ; ++j)
    {
        printf("\n -- final -- %d ", ((int *) *ind_val)[j] );
    }

    return 0;
}
////////////////////////////////////////////////////

