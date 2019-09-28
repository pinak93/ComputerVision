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
FILE *fpt,*fpt1,*fpt2; 
unsigned char *MSF; 
unsigned char *MSF_Binary;
unsigned char *Image; 
unsigned char *Thin_Image; 	
int i=0,r=0,c=0;
char T_header[80]; 
int T_ROWS,T_COLS,T_BYTES,Threshold=atoi(argv[1]);
char * line = NULL;
size_t len = 0;
ssize_t read;	
int bp=0,ep=0;
fpt=fopen("msf.ppm","r"); 
fpt1=fopen("parenthood.ppm","r");  
fpt2=fopen("parenthood_gt.txt","r");	
    
i=fscanf(fpt,"%s %d %d %d",T_header,&T_COLS,&T_ROWS,&T_BYTES); 
MSF=(unsigned char *)calloc(T_ROWS*T_COLS,sizeof(unsigned char));
MSF_Binary=(unsigned char *)calloc(T_ROWS*T_COLS,sizeof(unsigned char));
Image=(unsigned char *)calloc(T_ROWS*T_COLS,sizeof(unsigned char));
Thin_Image=(unsigned char *)calloc(T_ROWS*T_COLS,sizeof(unsigned char));
	T_header[0]=fgetc(fpt);
fread(MSF,1,T_ROWS*T_COLS,fpt); 
fclose(fpt);   
	
T_header[0]=fgetc(fpt);
fread(Image,1,T_ROWS*T_COLS,fpt1); 
fclose(fpt1);   
	
int TP=0,FP=0;
int TN=0,FN=0;
	
  while ((read = getline(&line, &len, fpt2)) != -1) {
  int pixelc=0;
  int pixelr=0;
	char *token = strtok(line, " "); 
    token = strtok(NULL, " "); 
  	pixelc=atoi(token);
	token = strtok(NULL, " ");   
	pixelr=atoi(token);
	
  int Flag=0;
	  
  for(r=-7;r<=7;r++){
    for(c=-4;c<=4;c++){	
	
		if(MSF[(pixelr+r)*T_COLS+(pixelc+c)]>=Threshold){
			Flag=1;
			break;
		}
	
	}
	if(Flag==1){break;}  
  }	
	  
 
	if(Flag==1){  
	
for(r=-7;r<=7;r++){
    for(c=-4;c<=4;c++){	
			if(Image[(pixelr+r)*T_COLS+(pixelc+c)]<128){	MSF_Binary[(pixelr+r)*T_COLS+(pixelc+c)]=255;	}
				else{MSF_Binary[(pixelr+r)*T_COLS+(pixelc+c)]=0;}
			}
  		}
		
	int Thining=8;	
while(Thining>1){
		for(r=-7;r<=7;r++){
			for(c=-4;c<=4;c++){	
			int C=pixelc+c;
			int R=pixelr+r;
			int CA=0;
			int CB=0;
			int A1=0,B1=0,C1=0,D1=0;
			if(MSF_Binary[R*T_COLS+C]==255){
				if(MSF_Binary[(R-1)*T_COLS+(C-1)]==255){
					if(MSF_Binary[(R-1)*T_COLS+(C)]==0){
						CA+=1;
					}
					CB+=1;
				}
				if(MSF_Binary[(R-1)*T_COLS+(C)]==255){
					if(MSF_Binary[(R-1)*T_COLS+(C+1)]==0){
						CA+=1;
					}
					CB+=1;A1=1;
				}
				if(MSF_Binary[(R-1)*T_COLS+(C+1)]==255){
					if(MSF_Binary[(R)*T_COLS+(C+1)]==0){
						CA+=1;
					}
					CB+=1;
				}
				if(MSF_Binary[(R)*T_COLS+(C+1)]==255){
					if(MSF_Binary[(R+1)*T_COLS+(C+1)]==0){
						CA+=1;
					}
					CB+=1;B1=1;
				}	
				if(MSF_Binary[(R+1)*T_COLS+(C+1)]==255){
					if(MSF_Binary[(R+1)*T_COLS+(C)]==0){
						CA+=1;
					}
					CB+=1;
				}	
				if(MSF_Binary[(R+1)*T_COLS+(C)]==255){
					if(MSF_Binary[(R+1)*T_COLS+(C-1)]==0){
						CA+=1;
					}
					CB+=1;D1=1;
				}
				if(MSF_Binary[(R+1)*T_COLS+(C-1)]==255){
					if(MSF_Binary[(R)*T_COLS+(C-1)]==0){
						CA+=1;
					}
					CB+=1;
				}
				if(MSF_Binary[(R)*T_COLS+(C-1)]==255){
					if(MSF_Binary[(R-1)*T_COLS+(C-1)]==0){
						CA+=1;
					}
					CB+=1;C1=1;
				}	
				if(CA==1 && (CB>=3 && CB<=7) && (A1==0 || B1==0 || (C1==0 && D1==0))){
					Thin_Image[R*T_COLS+C]=0;
					}else{
					Thin_Image[R*T_COLS+C]=255;
					}

			}
				else{
				Thin_Image[R*T_COLS+C]=0;
			}
		}
	}///FOR LOOP
	Thining--;
}////THINING WHILE
bp=0;
ep=0;
for(r=-7;r<=7;r++){
    for(c=-4;c<=4;c++){	
	int C=pixelc+c;
	int R=pixelr+r;
	if(Thin_Image[R*T_COLS+C]==255){
		int count=0;	
			if(Thin_Image[(R-1)*T_COLS+C]==255){count++;}
			if(Thin_Image[(R)*T_COLS+(C+1)]==255){count++;}
			if(Thin_Image[(R+1)*T_COLS+C]==255){count++;}
			if(Thin_Image[(R)*T_COLS+(C-1)]==255){count++;}
			
		if(count>=3){
			bp++;
		}
			if(Thin_Image[(R-1)*T_COLS+(C-1)]==255){count++;}
			if(Thin_Image[(R-1)*T_COLS+(C+1)]==255){count++;}
			if(Thin_Image[(R+1)*T_COLS+(C+1)]==255){count++;}
			if(Thin_Image[(R+1)*T_COLS+(C-1)]==255){count++;}
		
	
		if(count==1){
			ep++;
		}
		
		}
	}
}

	if(bp==1 && ep==1 && line[0]=='e'){
	TP++;
	}
	if(bp==1 && ep==1 && line[0]!='e'){
	FP++;
	}
	if((bp!=1 || ep!=1) && line[0]=='e'){
	FN++;
	}	
	if((bp!=1 || ep!=1) && line[0]!='e'){
	TN++;
	}

		
	}//Flag==1	
else{
	if(line[0]=='e'){
	FN++;
	}	
	if(line[0]!='e'){
	TN++;
	}	
} 
	  
	  
//printf("%c %d %d %d %d %d %d %d %d\n",line[0],Flag,bp,ep,TP,FP,FN,TN,TP+FP+FN+TN);	  
	  
	  
	  
}//While Loop
		printf("%d %d %d %d %d\n",TP,FP,FN,TN,TP+FP+FN+TN);	
	
	
fpt=fopen("Image.ppm","w"); 
fprintf(fpt,"P5 %d %d 255\n",T_COLS,T_ROWS); 
fwrite(MSF_Binary,1,T_ROWS*T_COLS,fpt); 
fclose(fpt); 
	
fpt=fopen("Thined.ppm","w"); 
fprintf(fpt,"P5 %d %d 255\n",T_COLS,T_ROWS); 
fwrite(Thin_Image,1,T_ROWS*T_COLS,fpt); 
fclose(fpt); 
		
}
	
