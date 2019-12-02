#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>
#include <string.h> 
#include <math.h>

#define THRESHOLD 137
#define ANGLE 0.65
#define MAX_QUEUE 10000
#define PIXEL_WITDH 3
// REGION GROW
int queue_paint_full(unsigned char *image, unsigned char *paint_image, int rows, int cols, 
					int current_row, int current_col, 
					int paint_over_label, int new_label,
					double *x, double *y, double *z)
{
	// VARIABLE DECLARATION SECTION
	int count;
	int	r2,c2;
    int	queue[MAX_QUEUE],qh,qt;
	int index;
	double dot_product;
	double average_surface_X, average_surface_Y, average_surface_Z;
	double angle = 0;
	double distance_A = 0;
	double distance_B = 0;
	double total[3] = {0, 0, 0};

    count = 0;

	// STARTING AVERAGE SURANCE NORMALS (AT CURRENT PIXEL VALUE)
	index = (current_row * cols) + current_col;
	average_surface_X = x[index];
	average_surface_Y = y[index];
	average_surface_Z = z[index];

	// STARTING TOTAL SUM VALUES (AT CURRENT PIXEL VALUE)
	total[0] = x[index];
	total[1] = y[index];
	total[2] = z[index];

    queue[0] = index;
    qh = 1;	/* queue head */
    qt = 0;	/* queue tail */
    count = 1;

    while (qt != qh)
    {
        for (r2 = -1; r2 <= 1; r2++)
        {
            for (c2 = -1; c2 <= 1; c2++)
            {

				index = (queue[qt] / cols + r2) * cols + queue[qt] % cols + c2;

				// PREDICATES TO SKIP
                if (r2 == 0  &&  c2 == 0)
                {
                    continue;
                }
				if ((queue[qt] / cols + r2) < 0  ||  (queue[qt] / cols + r2) >= rows - PIXEL_WITDH  ||
                (queue[qt] % cols + c2) < 0  ||  (queue[qt] % cols + c2) >= cols - PIXEL_WITDH)
                {
                    continue;
				} 
				if (paint_image[index] != 0)
				{
					continue;
				}

				// CALCULATES DOT PRODUCT
				dot_product = (average_surface_X * x[index]) + (average_surface_Y * y[index]) + (average_surface_Z * z[index]);
				
				// CALCULATES ANGLE 
				distance_A = sqrt( pow(average_surface_X, 2) + pow(average_surface_Y, 2) + pow(average_surface_Z,2) );
				distance_B = sqrt( pow(x[index], 2) + pow(y[index],2 ) + pow(z[index], 2) );  
				angle = acos(dot_product / (distance_A * distance_B));

				// PREDICATE WHICH DETERMINES IF CURRENT PIXEL IS IN THE SAME REGION
				if (angle > ANGLE)
				{
					continue;
				}

				// IF PIXEL IN SAME REGION, ADDED TO THE REGION, AVERAGE SURFANCE FOR X, Y, AND Z CALCULATED,
				// AND PIXEL IS LABELED.
				count++;
				total[0] += x[index];
				total[1] += y[index];
				total[2] += z[index];
				average_surface_X = total[0] / count;
				average_surface_Y = total[1] / count;
				average_surface_Z = total[2] / count;
                paint_image[index] = new_label;
                          
                queue[qh] = (queue[qt] / cols + r2) * cols+ queue[qt] % cols + c2;
                
                qh = (qh + 1) % MAX_QUEUE;
                
                if (qh == qt)
                {
                    printf("Max queue size exceeded\n");
                    exit(0);
                }
            }
        }
        qt = (qt + 1 )% MAX_QUEUE;
    }

	printf("X: %lf, Y: %lf, Z: %lf\n", average_surface_X, average_surface_Y, average_surface_Z);
	return count;
}




int main () 
{ 
	
FILE *fpt,*ftr; 
unsigned char *image,*threshold,*final; 	
double *x,*y,*z;
double *X, *Y, *Z;	
int *image_inverse; 
char T_header[80]; 
int ROWS,COLS,T_BYTES;
int	r,c;
double	cp[7];
double	xangle,yangle,dist;
double	ScanDirectionFlag=1,SlantCorrection;
double		P[3][128*128];
int             ImageTypeFlag;	
	
fpt=fopen("chair-range.ppm","r"); 
ftr=fopen("chair-reflectance.ppm","r");	
int i=0;
	int j=0;
i=fscanf(fpt,"%s %d %d %d",T_header,&COLS,&ROWS,&T_BYTES); 
	
image=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
threshold=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
final=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));	
x=(double *)calloc(ROWS*COLS,sizeof(double));
y=(double *)calloc(ROWS*COLS,sizeof(double));
z=(double *)calloc(ROWS*COLS,sizeof(double));
X=(double *)calloc(ROWS*COLS,sizeof(double));
Y=(double *)calloc(ROWS*COLS,sizeof(double));
Z=(double *)calloc(ROWS*COLS,sizeof(double));	
T_header[0]=fgetc(fpt);
fread(image,1,ROWS*COLS,fpt); 
fclose(fpt); 
	
	
	///Threhold
	for(int i=0;i<ROWS*COLS;i++){
	if (image[i] > THRESHOLD){
			threshold[i] = 255;
		}
		else{
			threshold[i] = image[i];
		}
	}
	//////XYZ
fpt=fopen("threshold.ppm","w"); 
fprintf(fpt,"P5 %d %d 255\n",COLS,ROWS); 
fwrite(threshold,1,ROWS*COLS,fpt); 
fclose(fpt); 	
	
	
	// COORDINATES ALGORITHM
	cp[0]=1220.7;		/* horizontal mirror angular velocity in rpm */
    cp[1]=32.0;		/* scan time per single pixel in microseconds */
    cp[2]=(COLS/2)-0.5;		/* middle value of columns */
    cp[3]=1220.7/192.0;	/* vertical mirror angular velocity in rpm */
    cp[4]=6.14;		/* scan time (with retrace) per line in milliseconds */
    cp[5]=(ROWS/2)-0.5;		/* middle value of rows */
    cp[6]=10.0;		/* standoff distance in range units (3.66cm per r.u.) */

    cp[0]=cp[0]*3.1415927/30.0;	/* convert rpm to rad/sec */
    cp[3]=cp[3]*3.1415927/30.0;	/* convert rpm to rad/sec */
    cp[0]=2.0*cp[0];		/* beam ang. vel. is twice mirror ang. vel. */
    cp[3]=2.0*cp[3];		/* beam ang. vel. is twice mirror ang. vel. */
    cp[1]/=1000000.0;		/* units are microseconds : 10^-6 */
    cp[4]/=1000.0;			/* units are milliseconds : 10^-3 */

	for (r=0; r<ROWS; r++)
    {
    for (c=0; c<COLS; c++)
      {
      SlantCorrection=cp[3]*cp[1]*((double)c-cp[2]);
      xangle=cp[0]*cp[1]*((double)c-cp[2]);
      yangle=(cp[3]*cp[4]*(cp[5]-(double)r))+SlantCorrection*ScanDirectionFlag;	/*  + slant correction */
      dist=(double)image[r*COLS+c]+cp[6];
      z[r*COLS+c]=sqrt((dist*dist)/(1.0+(tan(xangle)*tan(xangle))+(tan(yangle)*tan(yangle))));
      x[r*COLS+c]=tan(xangle)*z[r*COLS+c];
      y[r*COLS+c]=tan(yangle)*z[r*COLS+c];
      }
    }
	
	double x1, x2, y1, y2, z1, z2;
		int index1, index2, index3;

	for (i = 0; i < (ROWS - PIXEL_WITDH); i++)
	{
		for (j = 0; j < (COLS - PIXEL_WITDH); j++)
		{
			index1 = (i * COLS) + j;
			index2 = ((i + PIXEL_WITDH) * COLS) + j;
			index3 = (i * COLS) + (j + PIXEL_WITDH);

			// VECTOR A
			x1 = x[index3] - x[index1];
			y1 = y[index3] - y[index1];
			z1 = z[index3] - z[index1];

			// VECTOR B
			x2 = x[index2] - x[index1];
			y2 = y[index2] - y[index1];
			z2 = z[index2] - z[index1];

			// CROSS PRODUCT CALCULATION
			x[index1] = (y1 * z2) - (z1 * y2);
			y[index1] = ((x1 * z2) - (z1 * x2)) * -1;
			z[index1] = (x1 * y2) - (y1 * x2); 
		}
	}
	
	
	
	
	int valid, regions=0, k;
int new_label = 30;
	
	int index=0,count=0;;
	/* REGION GROW  */
	for (i = 2; i < ROWS -PIXEL_WITDH; i++)
	{
		for (j = 2; j < COLS -PIXEL_WITDH; j++)
		{
			valid = 1;
			for (r = -2; r < 3; r++)
			{
				for (c = -2; c < 3; c++)
				{
					index = ((i + r) * COLS) + (j + c);
					if (threshold[index] == 255 || final[index] != 0)
					{
						valid = 0;
					}
				}
			}
			if (valid == 1)
			{
				count = queue_paint_full(image, final, ROWS, COLS, i, j, 255, new_label, x, y, z);
				// IF REGION NOT BIG ENOUGH, LABELING IS RESETTED AND NOT COUNTED
				if (count < 100)
				{
					for (k = 0; k < (ROWS * COLS); k++)
					{
						if (final[k] == new_label)
						{
							final[k] = 0;
						}
					}
				}
				else
				{
					regions++;
					new_label += 30;
					printf("Region: %d, Number of Pixels: %d\n", regions, count);
				}
			}

		}
	}
	fpt=fopen("final.ppm","w"); 
fprintf(fpt,"P5 %d %d 255\n",COLS,ROWS); 
fwrite(final,1,ROWS*COLS,fpt); 
fclose(fpt); 
	
	
	
}




