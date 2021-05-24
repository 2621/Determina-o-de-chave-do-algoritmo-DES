#include "key_generator.h"
#include <stdio.h>
#include <stdlib.h>
// #include <mpi.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include "des.h"

unsigned long long int intToBinary(unsigned long long int k) {
    if (k == 0) return 0;
    if (k == 1) return 1;
    return (k % 2) + 10 * intToBinary(k / 2);
}



void int64ToChar(char *result, unsigned long long int n) {
  unsigned char byte[57];
  int j=0;
  for (int i =56; i> 0; i--){
    byte[j] = ((n >> i) & 1);
    byte[j] += 48;
    j++;
  }
  byte[56] = '\0';
  strcpy(result, byte);
}

void intToBinaryChar(char *charKey, unsigned long long int k){
  int pos = 0;
  while(k){
    if(k & 1){
      charKey[pos] = '1';
    }else{
      charKey[pos] = '0';
    }
    pos++;
    k >>= 1;
  }
  charKey[pos] = '\0';
}

void keyToHexa(char* key, char* result){
  int k;
  int value=0;
  for(k=0;k<61;k=k+4){
    //calcula de quatro em quatro bits o valor correspondente
    value = pow(2,3)*(key[k]-48) + pow(2,2)*(key[k+1]-48) + pow(2,1)*(key[k+2]-48) + pow(2,0)*(key[k+3]-48);

    sprintf(&result[k/4],"%X",value);
    value=0;
  }
  result[16] = '\0';

}

int main(int argc, char **argv){
  char plainText[9], cipherText[9], a[3];
  int keySize, size = 1, myRank = 0, finish = 0, flag = 0;
  unsigned long long int myKey;
  unsigned int thereIsNextKey = 1;
  struct timeval start, end, deltatime;
  unsigned char result[65], charKey[65], possibleKey[65], key[17];
  MPI Request request;
  
  strcpy(plainText, argv[1]);
  strcpy(cipherText, argv[2]);
  strcpy(a, argv[3]);
  a[2] = 0;
  keySize = atoi(a);
    
  printf("Buscando a senha de %d bits que encripta %s em %s\n", keySize, plainText, cipherText);
  gettimeofday(&start, NULL);

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

  myKey = (unsigned long long int)((1ULL << keySize)/size) * myRank;

  while (myKey < (unsigned long long int)((1ULL << keySize)/size) * (myRank+1)){
    MPI_Irecv(&finish, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &request);
    int64ToChar(charKey, myKey);
    getKey(charKey, possibleKey);
    encryptDES(plainText, possibleKey, result);

    if (!strcmp(result, cipherText)){
      keyToHexa(possibleKey, key);
      printf("Chave em hexa: %s\n", key);
      gettimeofday(&end, NULL);
      printf("levou %ld egundos para concluir\n", ((end.tv_sec) -
      (start.tv_sec)));
      finish = 1;
      for(i=0; i<npes; i++){
        MPI_Send(&finish, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
      }
      break;
    }
    
     if(finish){
      break;
     }
   
     myKey++;
  }

  MPI_Finalize();
  return 0;
}
