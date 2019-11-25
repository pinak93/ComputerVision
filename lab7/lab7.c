#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>
#include <string.h> 
#include <math.h>

#define window 10
#define AccThreshold 0.0009
#define GyroThreshold 0.03
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


float Var(float *smooth, int index){
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
	
FILE *ftr; 
char * line = NULL;
size_t len = 0;
ssize_t read;
ftr=fopen("acc_gyro.txt","r");	
	
int size=1250;
float *time,*x_acc,*y_acc,*z_acc,*pitch,*roll,*yaw;
float *Smooth_x_acc,*Smooth_y_acc,*Smooth_z_acc,*Smooth_pitch,*Smooth_roll,*Smooth_yaw;
	
int Smooth_window=20;
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
	vel=(float *)calloc(3,sizeof(float));
	Acc_movement=(float *)calloc(3,sizeof(float)); 	
	gyro_movement=(float *)calloc(3,sizeof(float)); 	
	//Read Contour Points
	int File_size=0;
	read = getline(&line, &len, ftr);
  while ((read = getline(&line, &len, ftr)) != -1){
	sscanf( line, "%f %f %f %f %f %f %f\n", &time[File_size],&x_acc[File_size],&y_acc[File_size],&z_acc[File_size],&pitch[File_size],&roll[File_size],&yaw[File_size]);
  File_size++;
  }	
	///Smoothing
	smooth(Smooth_window,size,x_acc,Smooth_x_acc);
	smooth(Smooth_window,size,y_acc,Smooth_y_acc);
	smooth(Smooth_window,size,z_acc,Smooth_z_acc);
	smooth(Smooth_window,size,pitch,Smooth_pitch);
	smooth(Smooth_window,size,roll,Smooth_roll);
	smooth(Smooth_window,size,yaw,Smooth_yaw);
	
	ftr=fopen("Smooth.txt","w");
	for(int i=0;i<File_size;i++){	
	fprintf(ftr,"%f %f %f %f %f %f\n",Smooth_x_acc[i],Smooth_y_acc[i],Smooth_z_acc[i],Smooth_pitch[i],Smooth_roll[i],Smooth_yaw[i]); 
	}
	int moving_now=0;
	int Start=-1;
	int end=-1;
	float Now_time;
	for(int i=0;i<File_size;i++){
	total=(float *)calloc(6,sizeof(float));
	vel=(float *)calloc(3,sizeof(float));
	Acc_movement=(float *)calloc(3,sizeof(float)); 	
	gyro_movement=(float *)calloc(3,sizeof(float)); 
			Now_time=time[i];
		float Var_xacc=Var(x_acc,i);
		float Var_yacc=Var(y_acc,i);
		float Var_zacc=Var(z_acc,i);
		float Var_pitch=Var(pitch,i);
		float Var_roll=Var(roll,i);
		float Var_yaw=Var(yaw,i);
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
		//gyro	
			for(int i=Start;i<end;i++){
		gyro_movement[0]=pitch[i]*sample_windowtime;
		gyro_movement[1]=roll[i]*sample_windowtime;
		gyro_movement[2]=yaw[i]*sample_windowtime;	
			}
		///acc	
			for(int i=Start;i<end;i++){
		float old = vel[0];		
		vel[0] += (x_acc[i] * Gravity * sample_windowtime);
		Acc_movement[0] += (((vel[0] + old) / 2) * sample_windowtime);	
			
		 old = vel[1];		
		vel[1] += (y_acc[i] * Gravity * sample_windowtime);
		Acc_movement[1] += (((vel[1] + old) / 2) * sample_windowtime);	
				
		 old = vel[2];		
		vel[2] += (z_acc[i] * Gravity * sample_windowtime);
		Acc_movement[2] += (((vel[2] + old) / 2) * sample_windowtime);			
			}		
	printf("Start Time-%f End Time-%f %f %f %f %f %f %f\n",time[Start],time[end],gyro_movement[0],gyro_movement[1],gyro_movement[2],Acc_movement[0],Acc_movement[1],Acc_movement[2]);		
	
		Start=-1;
			end=-1;
			moving_now=0;	
		}////calculate
			moving_now=0;
		
		
		
	}////Main For
	
}




