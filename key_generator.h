#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void extendWord(char *word, char ret[57]){
	char temp[57];
  int i = 0;
	// temp = (char *)malloc((56+1)*sizeof(char));
	for (; i < 56 - strlen(word); i++){
		temp[i] = '0';
	}
  temp[i] = '\0';
	strcat(temp, word);
	strcpy(ret, temp);
	// free(temp);
}

void generateKey(char *word, char* key){
	char byte[9];
	char temp[65];
	temp[0] = '\0';

	// temp = malloc((64+1)*sizeof(char));
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 7; j++){
			byte[j] = word[i*7+j];
		}
		int count1 = 0;
		for (int k = 0; k < 7; k++){
			if (byte[k] == '1'){
				count1++;
			}
		}
		if (count1 % 2 == 0){
			byte[7] = '1';
		} else {
			byte[7] = '0';
		}
		byte[8] = '\0';
		strcat(temp, byte);		
	}
	strcpy(key, temp);
	// free(temp);
}

int getKey (char word[65], char key[65]){
	// char *word = "1111110101010100"; //FD54
	char extendedWord[57];
	// extendedWord = malloc((56+1) * sizeof(char));
	extendWord(word, extendedWord);
	generateKey(extendedWord, key);
	// free(extendedWord);
	return 0;
}