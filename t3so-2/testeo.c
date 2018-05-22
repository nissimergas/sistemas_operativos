/* main.c */
#include <stdio.h>
#include <stdlib.h>


void char2binstr(char num, char* bin);

int main(int argc, char *argv[])
{
  char * cada_char = (char *) calloc(1024*8, sizeof(char));
  char2binstr('a', cada_char);
  free(cada_char);
  return 0;
}

void char2binstr(char num, char* bin){
	int i;
	for(i = 0; i<8; i++){
		bin[i] = '0';
	}

	int resto;
	i = 7;
	while(num != 0){
		if(i >= 0){
		resto = num %2;
		num = (num - resto) / 2;
		if(resto == 0){
			bin[i] = '0';
		}
		else{
			bin[i] = '1';
		}
		i--;
	}
}
}
