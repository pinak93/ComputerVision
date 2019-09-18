#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
int main()

{
FILE		*fpt;
unsigned char	*image;
unsigned char	*smoothed;
unsigned int *temp;
	char		header[320];

int		ROWS,COLS,BYTES;
int		r,c,r2,c2,sum;
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
for (r=0; r<ROWS; r++)
  for (c=0; c<COLS; c++)
    {
    sum=0;  
	  for (c2=-3; c2<=3; c2++){
		  sum+=  image[r*COLS+(c+c2)];
	 	}
   temp[r*COLS+c]=sum;
		  
  }
	
for(r=3; r<ROWS-3; r++)
  for(c=3; c<COLS-3; c++)
    {
    sum=0;
      
	  for (r2=-3; r2<=3; r2++){
			  sum+=  temp[((r+r2)*COLS+c)];
		}
	  
	  smoothed[r*COLS+c]=sum/49;
  
  
  }
	/* query timer */
clock_gettime(CLOCK_REALTIME,&tp2);

	/* report how long it took to smooth */
printf("%ld\n",tp2.tv_nsec-tp1.tv_nsec);

	/* write out smoothed image to see result */
fpt=fopen("SeptFilter.ppm","w");
fprintf(fpt,"P5 %d %d 255\n",COLS,ROWS);
fwrite(smoothed,COLS*ROWS,1,fpt);
fclose(fpt);
	return 0;
}

