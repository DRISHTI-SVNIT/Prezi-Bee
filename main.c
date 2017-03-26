/*

 * Author : Abhinav Jain
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "TWI_8.h"
#include "USART_32.h"
#define  F_CPU 1000000UL
#include <util/delay.h>
#include <stdint.h>

typedef struct kf
{
	float q; //process noise covariance
	float r; //measurement noise covariance
	float x; //value
	float p; //estimation error covariance
	float k; //kalman gain
}kalman_state;


int ACCEL_XOUT;
int ACCEL_YOUT;
int ACCEL_ZOUT;

int8_t ACCEL_XOUT_L;
int8_t ACCEL_XOUT_H;
int8_t ACCEL_YOUT_L;
int8_t ACCEL_YOUT_H;
int8_t ACCEL_ZOUT_L;
int8_t ACCEL_ZOUT_H;


float ACCEL_XOFFSET;
float ACCEL_YOFFSET;
float ACCEL_ZOFFSET;
float x,y,z;
float diffx=0;
float diffy=0;
float diffz=0;
float px=0;
float py=0;
float pz=0;
int r =0;
kalman_state accx,accy,accz,*statez = &accz, *statex = &accy, *statey = &accy;
void kalman_init(kalman_state *, float qq , float rr, float pp, float inital_value);
void kalman_update(kalman_state *state, float measurement);
void MPU_setup();
void Acceleration_config();
void kalman_start();
float Get_Accel_X();
float Get_Accel_Y();
float Get_Accel_Z();

#include <avr/io.h>


int main(void)
{
	USART_TransmitString("GO");
	USART_Init(6);
	TWIinit();
	MPU_setup();
	Acceleration_config();
	kalman_init(statex,0.125,32,1023,0);  //initialise Kalman
	kalman_init(statey,0.125,32,1023,0);
	kalman_init(statez,0.125,32,1023,0);
	for(int i=0;i<50;i++)
	{
		_delay_ms(20);
		kalman_update(statex, Get_Accel_X());
		kalman_update(statey, Get_Accel_Y());
		kalman_update(statez, Get_Accel_Z());
	}
	USART_TransmitString("OKAY!");
	while(1)
	{
		int i=0;
		while (1)
		{
			_delay_ms(250);
			kalman_update(statex, Get_Accel_X());
			kalman_update(statey, Get_Accel_Y());
			kalman_update(statez, Get_Accel_Z());
			diffx = (px-statex->x)*1000;     //dAx/dt
			diffy = (py-statey->x)*1000;	  //dAy/dt
			diffz = (pz-statez->x)*1000;	  //dAz/dt
			if(i==1)
			{
			
				if(diffx>7)// && (diffy < 0.2 && diffy > -0.2))   //Left  && (diffz<0.2 && diffz>-0.2)
				{	
					while(diffx>0)
					{
						kalman_update(statex, Get_Accel_X());
						kalman_update(statey, Get_Accel_Y());
						kalman_update(statez, Get_Accel_Z());
						if(r==0)
							{
							r=1;
							px=statex->x;
							py=statey->x;
							pz=statez->x;
							continue;	
						}
						else if(r==1)
						{
							diffx = (px-statex->x)*1000;     //dAx/dt
							diffy = (py-statey->x)*1000;	  //dAy/dt
							diffz = (pz-statez->x)*1000;	  //dAz/dt	
						}
						_delay_ms(50);
					}
					r=0;
					USART_Transmitchar('d');
					USART_Transmitchar(0x0D);
					_delay_ms(1000);
					break;
				}
				
				if((-1)*diffx>7)// && (diffz<0.2 && diffz>-0.2)) // Right && (diffy < 0.2 && diffy > -0.2)
				{
				while(diffx<0)
				{
					kalman_update(statex, Get_Accel_X());
					kalman_update(statey, Get_Accel_Y());
					kalman_update(statez, Get_Accel_Z());
					if(r==0)
					{
						r=1;
						px=statex->x;
						py=statey->x;
						pz=statez->x;
						continue;
					}
					else if(r==1)
					{
						diffx = (px-statex->x)*1000;     //dAx/dt
						diffy = (py-statey->x)*1000;	  //dAy/dt
						diffz = (pz-statez->x)*1000;	  //dAz/dt
					}
				_delay_ms(50);
				}
				r=0;
				USART_Transmitchar('a');
				USART_Transmitchar(0x0D);
				_delay_ms(1000);
				break;
				}
				 /*
				if(diffy>0.4 && (diffx < 0.2 && diffx > -0.2))   //Up  && (diffz<0.7 && diffz>-0.7)
				{
					
					USART_Transmitchar('s');
					USART_Transmitchar(0x0D);
					_delay_ms(500);
					break;
				}
				
				if((-1)*diffy>0.4 && (diffx < 0.2 && diffx > -0.2))//&& (diffz<0.7 && diffz>-0.7)) Down
				{
					
					USART_Transmitchar('w');
					USART_Transmitchar(0x0D);
					_delay_ms(500);
					break;
				}
			 */ /*
				if(diffz>0.  && (diffx<0.2 && diffx>-0.2))   //Left && (diffy < 0.2 && diffy > -0.2)
				{
					
					USART_Transmitchar('s',1);
					USART_Transmitchar(0x0D,1);
					_delay_ms(500);
					break;
				}
				
				if((-1)*diffz>0.1 && (diffx<0.2 && diffx>-0.2)) //&& (diffy < 0.2 && diffy > -0.2)
				{
					
					USART_Transmitchar('w',1);
					USART_Transmitchar(0x0D,1);
					_delay_ms(500);
					break;
				}
				*/
			}
	    	i=1;
	    	px=statex->x; 
	    	py=statey->x;
	    	pz=statez->x;
		}
	}


}


void Acceleration_config()
{
	
	float ACCEL_XOFFSETSUM=0,ACCEL_YOFFSETSUM=0, ACCEL_ZOFFSETSUM=0;
	for(int i =0;i<100;i++)
	{
		ACCEL_XOFFSETSUM +=Get_Accel_Y();
		ACCEL_YOFFSETSUM +=Get_Accel_Y();
		ACCEL_ZOFFSETSUM += Get_Accel_Z();
	}
	
	ACCEL_ZOFFSET=ACCEL_ZOFFSETSUM/100.0;
	ACCEL_XOFFSET=ACCEL_XOFFSETSUM/100.0;
	ACCEL_YOFFSET=ACCEL_YOFFSETSUM/100.0;
}

void MPU_setup()
{
	TWITransmit(0x68,0x6B,0x00);
	TWITransmit(0x68,0x6C,0x00);
	TWITransmit(0x68,0x1B,0x18);
	TWITransmit(0x68,0x19,0x00);
}

float Get_Accel_X()
{
	ACCEL_XOUT = TWIReadNack(0x68,0x3B);
	ACCEL_XOUT = ACCEL_XOUT<<8;
	ACCEL_XOUT |= TWIReadNack(0x68,0x3C);
	float a= ((float)ACCEL_XOUT/16384)-ACCEL_XOFFSET;
	return a; //***************************Not updating Kalman
}

float Get_Accel_Y()
{
	
	ACCEL_YOUT = TWIReadNack(0x68,0x3D);
	ACCEL_YOUT = ACCEL_YOUT<<8;
	ACCEL_YOUT |= TWIReadNack(0x68,0x3E);
	float a=(float)ACCEL_YOUT/(16384)-ACCEL_YOFFSET;
	//kalman_update(statey, a);
	return a;
}

float Get_Accel_Z()
{
	ACCEL_ZOUT = TWIReadNack(0x68,0x3F);
	ACCEL_ZOUT = ACCEL_YOUT<<8;
	ACCEL_ZOUT |= TWIReadNack(0x68,0x40);
	float a = (float)ACCEL_ZOUT/(16384)-ACCEL_ZOFFSET;
	//kalman_update(statez, a);
	return a;
}

void kalman_init(kalman_state* s, float qq , float rr, float pp, float intial_value)
{
	
	s->q = qq;
	s->r = rr;
	s->p = pp;
	s->x = intial_value;
	s->k=0;
}

void kalman_update(kalman_state *state, float measurement)
{
	//prediction update
	//omit x = x
	state->p=state->p+state->q;
	//measurement update
	state->k=state->p/(state->p + state->r);
	state->x=state->x+state->k*(measurement-state->x);
	state->p=(1-state->k)*state->p;
}
