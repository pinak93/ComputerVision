#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>
#include <string.h> 
int main (int argc, char *argv[]) 
{ 
	if (argc!=2)  
    { 
        printf("Enter Threshold"); 
        return 0; 
    } 
FILE *fpt2,*fpt,*ftr; 
unsigned char *template; 
unsigned char *image; 
int *MSF;  
int *MSF_Image;	
char header[80]; 
int ROWS,COLS,BYTES;
char T_header[80]; 
int T_ROWS,T_COLS,T_BYTES;
int i,j,Threshold=atoi(argv[1]); 
int r=0,c=0;
int sum=0,total=0; 
int max=0;
int counter=0;
int r1=0,c1=0;

char * line = NULL;
size_t len = 0;
ssize_t read;
	
fpt2=fopen("parenthood_e_template.ppm","r"); 
fpt=fopen("parenthood.ppm","r");  
ftr=fopen("parenthood_gt.txt","r");	
if (fpt2 == NULL || fpt == NULL) {   
  printf("Unable to open for reading\n"); 
  exit(0); 
  }
    
 i=fscanf(fpt2,"%s %d %d %d",T_header,&T_COLS,&T_ROWS,&T_BYTES); 
 j=fscanf(fpt,"%s %d %d %d",header,&COLS,&ROWS,&BYTES); 
/* allocate dynamic memory for image */ 
template=(unsigned char *)calloc(T_ROWS*T_COLS,sizeof(unsigned char)); 
image=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char)); 
MSF=(int *)calloc(T_ROWS*T_COLS,sizeof(int)); 
MSF_Image=(int *)calloc(ROWS*COLS,sizeof(int)); 


if (template == NULL || image==NULL || MSF==NULL) {   
  printf("Calloc Failed\n"); 
  exit(0); 
  } 
  T_header[0]=fgetc(fpt2);
  fread(template,1,T_ROWS*T_COLS,fpt2); 
  
  header[0]=fgetc(fpt);
  fread(image,1,ROWS*COLS,fpt); 
fclose(fpt2);   
 fclose(fpt);
  
 ///creating MSF Image 
  for(r=0;r<T_ROWS;r++){
    for(c=0;c<T_COLS;c++){
    sum+=template[r*T_COLS+c];
    }
  }
 total=(T_COLS*T_ROWS); 
sum=sum/total;
   for(r=0;r<T_ROWS;r++){
    for(c=0;c<T_COLS;c++){
    MSF[r*T_COLS+c]=template[r*T_COLS+c]-sum;
    }
}	
	
  i=(T_ROWS/2);
  j=(T_COLS/2);
  sum=0;

   for(r=i;r<ROWS-i;r++){
    for(c=j;c<COLS-j;c++){
 
      int i1=0,j1=0;
      for(r1=-i;r1<=i;r1++){
        j1=0;
        for(c1=-j;c1<=j;c1++){
         
             sum+=(MSF[i1*T_COLS+j1]*image[(r+r1)*COLS+(c+c1)]); 
        j1++;
        }
		  i1++;
       }
	if(max<sum){
	max=sum;
	}
	
		MSF_Image[r*COLS+c]=sum;
		sum=0;
	}
   }

	
	
   for(r=i;r<ROWS-i;r++){
    for(c=j;c<COLS-j;c++){
     
      int finalsum=(unsigned int)(((double)MSF_Image[r*COLS+c]/(double)max)*255);
      MSF_Image[r*COLS+c]=finalsum;
		}
	 
   }

int TP=0,FP=0;
int TN=0,FN=0;	
	
  while ((read = getline(&line, &len, ftr)) != -1) {
  int pixelc=0;
  int pixelr=0;
	char *token = strtok(line, " "); 
    token = strtok(NULL, " "); 
  	pixelc=atoi(token);
	token = strtok(NULL, " ");   
	pixelr=atoi(token);
	
  int Flag=0;
	  
  for(r=-i;r<=i;r++){
    for(c=-j;c<=j;c++){	
		if(MSF_Image[(pixelr+r)*COLS+(pixelc+c)]>=Threshold){
			if(line[0]=='e'){
			TP++;
			Flag=1;
			}
			else{
			FP++;
			Flag=1;
			}
			
			break;
		}
	 }
	  if(Flag==1){break;}  ///Leave if Flag is set and a value above threshold is found
}
	  ///Enter if no no value is above Threshold
 	 if(Flag==0){
  			if(line[0]=='e'){
			FN++;
			}
			else{
			TN++;
			} 
  }
	  Flag=0;
  }///End While
	printf("TP-%d FP-%d TN-%d FN-%d  Total-%d Threshold-%d\n",TP,FP,TN,FN,TP+FP+TN+FN,Threshold);
} 
