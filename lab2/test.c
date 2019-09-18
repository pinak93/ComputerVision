#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>
int main () 
{ 
	FILE *ftr;
ftr=fopen("parenthood_gt.txt","r");	
	
char * line = NULL;
size_t len = 0;
ssize_t read;
int count=0; 
	while ((read = getline(&line, &len, ftr)) != -1) {
  
  if(line[0]=='e'){
  count++;
  }
  
  }
	printf("%d",count);
} 

