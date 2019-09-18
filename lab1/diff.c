#include <stdio.h> 
#include <stdlib.h> 
int main (int argc,char *argv[]) 
{ 
FILE *fpt1,*fpt2; 
unsigned char *image,*image2; 
char header[80]; 
int ROWS,COLS,BYTES; 
int i,j,b;  
	if (argc != 3) {   
  printf("Usage:  brighten [filename] [amount]\n"); 
  exit(0); 
  }
/* open image for reading */ 
fpt1=fopen(argv[1],"r"); 

fpt2=fopen(argv[2],"r");  
i=fscanf(fpt1,"%s %d %d %d",header,&COLS,&ROWS,&BYTES);   
image=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));  

	
i=fscanf(fpt2,"%s %d %d %d",header,&COLS,&ROWS,&BYTES);   
image2=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));  
	int counter=0;
fread(image,1,ROWS*COLS,fpt1);
fread(image2,1,ROWS*COLS,fpt2);
	for (i=0; i<ROWS*COLS; i++) {
		if(image2[i]-image[i]!=0){
		counter++;
  }
	}
	if(counter==0){
		printf("Images are Equal\n");
		}else
	{
		printf("Images not equal by %d pixel\n",counter);
	}
fclose(fpt1);
fclose(fpt2);
	
	
} 

