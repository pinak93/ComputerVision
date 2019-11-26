#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>
#include <string.h> 
#include <math.h>

#define AccThreshold 0.003
#define GyroThreshold 0.005
#define Gravity 9.81
#define sample_windowtime 0.05


void smooth(int Smooth_window,int size,float *raw,float *smooth){
		double sum=0;
	for (int i = 0; i < sample_windowtime; i++){
		smooth[i] = raw[i];
	}
	for (int i = sample_windowtime - 1; i < size; i++){
		for (int j = 1; j < sample_windowtime; j++){
			sum += raw[i-j];
		}
		smooth[i] = (sum+raw[i]) / sample_windowtime;
		sum = 0;
	}
}


float Var(float *smooth, int index,int window){
	float variance=0;
	float mean=0;
	float total=0;
	for(int i=index;i<index+window;i++){
		
		total=smooth[i]+total;
	
	}
	mean =total/window;
	for(int i=index;i<index+window;i++){
		variance=variance+pow((smooth[i]-mean),2);
	}
	variance=(variance/(window-1));
	return variance;
	
}



int main () 
{ 
	
FILE *ftr,*ftr1; 
char * line = NULL;
size_t len = 0;
ssize_t read;
ftr=fopen("acc_gyro.txt","r");	
	
int size=1250;
float *time,*x_acc,*y_acc,*z_acc,*pitch,*roll,*yaw;
float *Smooth_x_acc,*Smooth_y_acc,*Smooth_z_acc,*Smooth_pitch,*Smooth_roll,*Smooth_yaw;
int window=10;
time=(float *)calloc(size,sizeof(float)); 
	
x_acc=(float *)calloc(size,sizeof(float)); 
y_acc=(float *)calloc(size,sizeof(float)); 
z_acc=(float *)calloc(size,sizeof(float)); 
pitch=(float *)calloc(size,sizeof(float)); 	
roll=(float *)calloc(size,sizeof(float)); 	
yaw=(float *)calloc(size,sizeof(float)); 
///Smooth 
Smooth_x_acc=(float *)calloc(size,sizeof(float)); 
Smooth_y_acc=(float *)calloc(size,sizeof(float)); 
Smooth_z_acc=(float *)calloc(size,sizeof(float)); 
Smooth_pitch=(float *)calloc(size,sizeof(float)); 	
Smooth_roll=(float *)calloc(size,sizeof(float)); 	
Smooth_yaw=(float *)calloc(size,sizeof(float)); 	
float *Acc_movement,*gyro_movement;
	float *vel,*total;
	total=(float *)calloc(6,sizeof(float));	
	//Read Contour Points
	int File_size=0;
	read = getline(&line, &len, ftr);
  while ((read = getline(&line, &len, ftr)) != -1){
	sscanf( line, "%f %f %f %f %f %f %f\n", &time[File_size],&x_acc[File_size],&y_acc[File_size],&z_acc[File_size],&pitch[File_size],&roll[File_size],&yaw[File_size]);
  File_size++;
  }	

	///Smoothing
	smooth(window,size,x_acc,Smooth_x_acc);
	smooth(window,size,y_acc,Smooth_y_acc);
	smooth(window,size,z_acc,Smooth_z_acc);
	smooth(window,size,pitch,Smooth_pitch);
	smooth(window,size,roll,Smooth_roll);
	smooth(window,size,yaw,Smooth_yaw);
	
	ftr=fopen("Smooth.txt","w");
	for(int i=0;i<File_size;i++){	
	fprintf(ftr,"%f %f %f %f %f %f\n",Smooth_x_acc[i],Smooth_y_acc[i],Smooth_z_acc[i],Smooth_pitch[i],Smooth_roll[i],Smooth_yaw[i]); 
	}
	ftr=fopen("TotalData.txt","w");
	ftr1=fopen("event.txt","w");

	
	while(window<=50){
	fprintf(ftr1,"Accelerometer Threshold - %f	GyroScope Threshold - %f	Window - %d \n",AccThreshold,GyroThreshold,window);				
	total=(float *)calloc(6,sizeof(float));	
	int moving_now=0;
	int Start=-1;
	int end=-1;
	float Now_time;	
		
	for(int i=0;i<File_size;i++){
		////reload
		
	vel=(float *)calloc(3,sizeof(float));
	Acc_movement=(float *)calloc(3,sizeof(float)); 	
	gyro_movement=(float *)calloc(3,sizeof(float)); 
			Now_time=time[i];
		float Var_xacc=Var(x_acc,i,window);
		float Var_yacc=Var(y_acc,i,window);
		float Var_zacc=Var(z_acc,i,window);
		float Var_pitch=Var(pitch,i,window);
		float Var_roll=Var(roll,i,window);
		float Var_yaw=Var(yaw,i,window);
	//Check if in Rest
		if(Var_xacc>AccThreshold || Var_yacc>AccThreshold || Var_zacc>AccThreshold){
			moving_now=1;
		}
	 if(Var_pitch>GyroThreshold || Var_roll>GyroThreshold || Var_yaw>GyroThreshold){
			moving_now=1;

		}

		if(Start==-1 && moving_now==1){
		Start=i;
		}
		 if(Start!=-1 && moving_now==0){
		end=i;
		}
		if((i+1==File_size) && (Start!=-1 && moving_now==1))end=i;
	
		if(Start>=0 && end>0 && moving_now==0){
	
		///acc	
			for(int i=Start;i<end;i++){
		gyro_movement[0]=pitch[i]*sample_windowtime;
		gyro_movement[1]=roll[i]*sample_windowtime;
		gyro_movement[2]=yaw[i]*sample_windowtime;	
		
		float old = vel[0];		
		vel[0] += (x_acc[i] * Gravity * sample_windowtime);
		Acc_movement[0] += (((vel[0] + old) / 2) * sample_windowtime);	
			
		 old = vel[1];		
		vel[1] += (y_acc[i] * Gravity * sample_windowtime);
		Acc_movement[1] += (((vel[1] + old) / 2) * sample_windowtime);	
				
		 old = vel[2];		
		vel[2] += (z_acc[i] * Gravity * sample_windowtime);
		Acc_movement[2] += (((vel[2] + old) / 2) * sample_windowtime);	
fprintf(ftr,"%d %f %f %f %f %f %f \n",i,gyro_movement[0],gyro_movement[1],gyro_movement[2],Acc_movement[0],Acc_movement[1],Acc_movement[2]);				
			}	
			total[0]+=gyro_movement[0];
			total[1]+=gyro_movement[1];
			total[2]+=gyro_movement[2];
			
			total[3]+=Acc_movement[0];
			total[4]+=Acc_movement[1];
			total[5]+=Acc_movement[2];
fprintf(ftr1,"%d %d %d %f %f %f %f %f %f %f %f \n",Start,end,(end-Start),time[Start],time[end],gyro_movement[0],gyro_movement[1],gyro_movement[2],Acc_movement[0],Acc_movement[1],Acc_movement[2]);		
	
	

		Start=-1;
			end=-1;
			moving_now=0;	
		}////calculate
		else{
		fprintf(ftr,"%d %f %f %f %f %f %f \n",i,gyro_movement[0],gyro_movement[1],gyro_movement[2],Acc_movement[0],Acc_movement[1],Acc_movement[2]);		
		}
			moving_now=0;

	}////Main For
fprintf(ftr1,"%d %d %d %f %f %f %f %f %f %f %f \n",Start,end,(end-Start),time[Start],time[end],total[0],total[1],total[2],total[3],total[4],total[5]);	

fprintf(ftr1,"---------------------------------------------------------------------------------------------- \n");	
fprintf(ftr1,"---------------------------------------------------------------------------------------------- \n");	

		window+=10;
	}///while

}




