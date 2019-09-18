#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()

{
FILE		*fpt;
unsigned char	*image;
unsigned char	*smoothed;
unsigned int	*temp;
	char		header[320];

int		ROWS,COLS,BYTES;
int		r,c,r2,c2;
unsigned int sum;
struct timespec	tp1,tp2;

	/* read image */
if ((fpt=fopen("bridge.ppm","rb")) == NULL)
  {
  printf("Unable to open bridge.ppm for reading\n");
  exit(0);
  }
fscanf(fpt,"%s %d %d %d",header,&COLS,&ROWS,&BYTES);
image=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
header[0]=fgetc(fpt);	/* read white-space character that separates header */
fread(image,1,COLS*ROWS,fpt);
fclose(fpt);

	/* allocate memory for smoothed version of image */
smoothed=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
temp=(unsigned int *)calloc(ROWS*COLS,sizeof(unsigned int));
	
	/* query timer */
clock_gettime(CLOCK_REALTIME,&tp1);
	/* smooth image, skipping the border points */
  sum=0; 
	int flag=0;
for (r=0; r<ROWS; r++){
	  sum=0; flag=0;
  for (c=0; c<COLS; c++) {
  if(flag==0){
      for (c2=-3; c2<=3; c2++){
		  sum+=  image[r*COLS+(c+c2)];
	  		}
	 flag=1;
   		}
	  else
	  {
		  sum+=image[r*COLS+(c+3)];		  
	  }
   temp[r*COLS+c]=sum;
  	sum-=image[r*COLS+(c-3)]; 
	  }	  
}
	
for (c=3; c<COLS-3; c++){
	  sum=0; flag=0;
  for (r=3; r<ROWS-3; r++)
    {
	  
   if(flag==0){
      for (c2=-3; c2<=3; c2++){
		  sum+=  temp[((r+c2)*COLS+c)];
	  }
	  flag=1;
   }
	  else
	  {
		  sum+=temp[((r+3)*COLS+c)];
	  }
	
	  smoothed[r*COLS+c]=sum/49;
	sum-=temp[((r-3)*COLS+c)];
  


		  
		  }

}
	
	/* query timer */
clock_gettime(CLOCK_REALTIME,&tp2);

	/* report how long it took to smooth */
printf("%ld\n",tp2.tv_nsec-tp1.tv_nsec);

	/* write out smoothed image to see result */
fpt=fopen("Sliding.ppm","w");
fprintf(fpt,"P5 %d %d 255\n",COLS,ROWS);
fwrite(smoothed,COLS*ROWS,1,fpt);
fclose(fpt);
	return 0;
}

