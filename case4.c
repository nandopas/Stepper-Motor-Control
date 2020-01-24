//case4 group7

#include <xc.h>
#include <pic.h>

	#pragma config FOSC=HS, CP=OFF, DEBUG=OFF, BORV=20, BOREN=0, MCLRE=ON, PWRTE=ON, WDTE=OFF
	#pragma config BORSEN=OFF, IESO=OFF, FCMEN=0


/* Variable declarations */

#define PORTBIT(adr,bit) ((unsigned)(&adr)*8+(bit))
#define clockwise 0				// clockwise direction macro
#define counter_clockwise 1		// anti clockwise direction macro

// The function PORTBIT is used to give a name to a bit on a port
// The variable RC0 could have equally been used

	static bit greenButton @ PORTBIT(PORTC,0);
	static bit redButton @ PORTBIT(PORTC,1);

	char Mode, direction, k;
	int Sensor, Sensor4, Sensor5, Sensor6, Sensor7;
	int speed, temp, testsensor;



void SwitchDelay (void)					// Waits for switch debounce
{
	for (temp=200; temp > 0; temp--){}			// 1200 us delay
}



void initAtoD(void) 						// Initialize A/D
{
	ADCON1 = 0b1111; 						// RA0,RA1,RA2,RA3 analog inputs, rest digital
	ADCON0 = 0b01000001; 					// Select 8* oscillator, analog input 0, turn on
	SwitchDelay(); 							// Delay a bit before starting A/D conversion
	GO = 1; 								// Start A/D
}



void ms_delay(int speed)
{
     unsigned int i,j;
        for(i=0;i<speed;i++)
		{
            for(j=0;j<1650;j++){}
		}
}



void full_drive_uni(char direction)
{
	if (direction == clockwise)
	{
		PORTD = 0b00000011;
		ms_delay(speed);
		PORTD = 0b00000110;
		ms_delay(speed);
		PORTD = 0b00001100;
		ms_delay(speed);
		PORTD = 0b00001001;
		ms_delay(speed);
	}
   
	if (direction == counter_clockwise)
	{
		PORTD = 0b00001001;
		ms_delay(speed);
		PORTD = 0b00001100;
		ms_delay(speed);
		PORTD = 0b00000110;
		ms_delay(speed);
		PORTD = 0b00000011;
		ms_delay(speed);
	}
}



void full_drive_bi(char direction)
{
	if (direction == counter_clockwise)
	{
		PORTD = 0b00010000;
		ms_delay(speed);
		PORTD = 0b00000000;
		ms_delay(speed);
		PORTD = 0b01000000;
		ms_delay(speed);
		PORTD = 0b01010000;
		ms_delay(speed);
	}
   
	if (direction == clockwise)
	{
		PORTD = 0b01000000;
		ms_delay(speed);
		PORTD = 0b00000000;
		ms_delay(speed);
		PORTD = 0b00010000;
		ms_delay(speed);
		PORTD = 0b01010000;
		ms_delay(speed);
	}
}



int getSensor(void)
{
	temp = PORTB;
	
	return temp;
}


void bringHome(void)
{
	Sensor = getSensor();
	while((Sensor>>4&1) == 0)
	{
		full_drive_uni(counter_clockwise);
		Sensor = getSensor();
	}
	
	Sensor = getSensor();
	while((Sensor>>7&1) == 0)
	{
		full_drive_bi(clockwise);
		Sensor = getSensor();
	}
}


void bringHome3(void)
{
	Sensor = getSensor();
	while((Sensor>>5&1) == 0)
	{
		full_drive_uni(counter_clockwise);
		Sensor = getSensor();
	}
	
	Sensor = getSensor();
	while((Sensor>>7&1) == 0)
	{
		full_drive_bi(counter_clockwise);
		Sensor = getSensor();
	}
}




void main (void)
{


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	PORTB = 0b00000000; 				// Clear Port B output latches
	PORTC = 0b00000000; 				// Clear Port C output latches
	PORTD = 0b00000000; 				// Clear Port D output latches
	TRISB = 0b11110000; 				// Configure the first 4 pins on Port B as output and others input
	TRISC = 0b11111111; 				// Configure Port C as all input
	TRISD = 0b00000000; 				// Configure Port D as all output
	TRISE = 0b00000111; 				// Configure pin 0, 1 and 2 of PORTE as input
	
	initAtoD();


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

Start:
	while(1) 							// Infinite loop
	{
		PORTB = 0b00000000; 				// Clear Port B output latches
		PORTC = 0b00000000; 				// Clear Port C output latches
		PORTD = 0b00000000; 				// Clear Port D output latches
		TRISB = 0b11110000; 				// Configure the first 4 pins on Port B as output and others input
		TRISC = 0b11111111; 				// Configure Port C as all input
		TRISD = 0b00000000; 				// Configure Port D as all output
		TRISE = 0b00000111; 				// Configure pin 0, 1 and 2 of PORTE as input
		
		initAtoD();


		speed = 5;

		
		Sensor = getSensor();
		Sensor4 = Sensor>>4&1;

		Sensor = getSensor();
		Sensor7 = Sensor>>7&1;

		if((Sensor4&Sensor7) == 0)
		{
			bringHome();
		}
		
		if(greenButton == 1) 			// If green press...
		{
			while(greenButton == 1){} 	// Wait for release
			Mode = PORTE;
			Mode = Mode|0b11111000;
			Mode = ~Mode;
		
			
////////////Mode 1
			if(Mode == 1)
			{
				PORTB = 0b11110001;
				
				while(1)
				{
					Sensor = getSensor();
					Sensor4 = Sensor>>4&1;
	
					Sensor = getSensor();
					Sensor7 = Sensor>>7&1;
	
					
					if(greenButton == 1) 			// If green press...
					{
						goto Start;
					}
					
					
					if(redButton == 0)
					{
						continue;
					}
					
					while(redButton == 1){} 			// Wait for release
					
					
					// Check status
					Sensor = getSensor();
					Sensor4 = Sensor>>4&1;
	
					Sensor = getSensor();
					Sensor7 = Sensor>>7&1;
	
					if (Sensor4)
					{
						if (Sensor7)
						{
							k = 0;
						}
						
						else
						{
							k = 3;
						}
					}
					
					else
					{
						if (Sensor7)
						{
							k = 1;
						}
						
						else
						{
							k = 2;
						}
					}
					
					
					// Rotate
					if(k == 0)
					{
						while((Sensor>>5&1) == 0)
						{
							full_drive_uni(counter_clockwise);
							Sensor = getSensor();
						}
					}
					
					else if(k == 1)
					{
						while((Sensor>>6&1) == 0)
						{
							full_drive_bi(clockwise);
							Sensor = getSensor();
						}
					}
					
					else if(k == 2)
					{
						while((Sensor>>4&1) == 0)
						{
							full_drive_uni(clockwise);
							Sensor = getSensor();
						}
					}
					
					else if(k == 3)
					{
						while((Sensor>>7&1) == 0)
						{
							full_drive_bi(counter_clockwise);
							Sensor = getSensor();
						}
					}
					
				}
			}
			

////////////Mode 3
			if(Mode == 3)
			{
				PORTB = 0b11110011;

				Sensor = getSensor();
				Sensor5 = Sensor>>5&1;

				Sensor = getSensor();
				Sensor7 = Sensor>>7&1;

				if((Sensor5&Sensor7) == 0)
				{
					bringHome3();
				}
				
	
				while(1)
				{
					while(redButton == 0){}
					while(redButton == 1){}


					/*
					if(greenButton == 1) 			// If green press...
					{
						break;
					}
					
					else if(redButton == 0) 				// 
					{
						continue;
					}
					else
					{
						while(redButton == 1){} 		// Wait for release
					}
					*/
					
	
Move3:
					while(1)
					{

						Sensor = getSensor();
						Sensor4 = Sensor>>4&1;
	
						Sensor = getSensor();
						Sensor6 = Sensor>>6&1;
						testsensor = Sensor4 & Sensor6;
						while(((Sensor4)&(Sensor6)) == 0)
						{
							if(Sensor4 == 0)
							{
								full_drive_uni(counter_clockwise);
							}
							
							if(Sensor6 == 0)
							{
								full_drive_bi(counter_clockwise);
							}
							
							Sensor = getSensor();
							Sensor4 = Sensor>>4&1;
		
							Sensor = getSensor();
							Sensor6 = Sensor>>6&1;
						}
						
						
	
						Sensor = getSensor();
						Sensor5 = Sensor>>5&1;
	
						Sensor = getSensor();
						Sensor7 = Sensor>>7&1;
	
						while(((Sensor5)&(Sensor7)) == 0)
						{
							if(Sensor5 == 0)
							{
								full_drive_uni(clockwise);
							}
							
							if(Sensor7 == 0)
							{
								full_drive_bi(clockwise);
							}
	
							Sensor = getSensor();
							Sensor5 = Sensor>>5&1;
		
							Sensor = getSensor();
							Sensor7 = Sensor>>7&1;
						}


						if(redButton == 1)
						{
							break;
						}
						
					}
					
					while(redButton == 1){}
					
					while(1)
					{
						if(redButton == 1)
						{
							while(redButton == 1){}
							goto Move3;
						}
						
						if(greenButton == 1)
						{
							goto Start;
						}
					}
				}
				
			}

			
			if((Mode != 1)&(Mode != 3))
			{
				//error
				PORTD = 0b00000000;
				PORTB = 0b11111000;
				
				while(1){}
	
			}
		}
	
	}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

}
