/*Partner(s)Name & email: Antonio Martinez, amart108@ucr.edu
 * Lab Section
 *Assignment: Final project find direction and move around an object
 *Exercise Description: 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work
 * Author : Antonio 
 */
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; //TimerISR() sets this to 1. C programmer should clear to 0.
//Internal variable for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M  = 1; // start count from here, down to 0, default to 1 ms.
unsigned long _avr_timer_cntcurr = 0; // current internal count of 1ms ticks

void TimerOn(){
	TCCR1B = 0x0B;
	
	OCR1A = 125;
	
	TIMSK1 = 0x02;
	
	TCNT1 = 0;
	
	_avr_timer_cntcurr = _avr_timer_M;
	
	SREG |= 0x80;
	
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

int c1 = 0;
int c2 = 0;
int c3 = 0;
void Stop()
{
	//Stops the car
	PORTB = 0x00;
}

void ReverseLeft()
{
	// This move the left tire backwards so it moves in reverse
	//B = 1: Move backwards and to the left with left tire
	PORTB = 0x01;
}

void Right()
{
	//Moves the car to the right
	//B = 2: Turn right with left tire
	PORTB = 0x02;
}

void Left()
{
	//Move the car to the left
	//B = 4: Turn left with right tire
	PORTB = 0x04;
}

void Forward()
{
	// Moves the car straight forward
	//B = 6: Move forward
	PORTB = 0x06;
}

void ReverseRight()
{
	//B = 8:  Move backwards and to the right with right tire
	PORTB = 0x08;
}

void Reverse()
{
	//Moving the car straight backwards
	//B = 9: Move backwards
	PORTB = 0x09;	
}


int lcount = 0;
int lat = 0x00;
int Gps = 0x00; // tells if you are heading in the right direction//
enum LStates{start, move_f, read,waitG, found_dir, rev, moveleft}LState;
void LatDir()
{
	Gps = PINC & 0x03;
	switch(LState)//transitions
	{
		case start:
			LState = move_f;
		break;
		
		case move_f:
			if((lcount != 0x11))
			{
				LState = move_f;
			}
			else
			{
				LState = read;
				lcount = 0x00;
			}
		break;
		
		case read:
			if((lcount != 0x32))
			{
				LState = read;
			}
			else
			{
				LState = waitG;
				lcount = 0x00;
			}
		break;
		
		case waitG:
			if((Gps == 0x01))
			{
				//PORTA = 0x0F;
				LState = found_dir;
			}
			else
			{
				LState = rev;
			}
		
		case rev:
			if((lcount != 0x11))
			{
				LState = rev;
			}
			else
			{
				LState = moveleft;
				lcount = 0x00;
			}
		break;
		
		case moveleft:
			if((lcount != 0x08))
			{
				LState = moveleft;
			}
			else
			{
				LState = move_f;
				lcount = 0x00;
			}
		break;
		
		case found_dir:
			
			LState = found_dir;
		break;
		
		default:
			LState = start;
		break;
	}	
	switch(LState)//actions
	{
		case start:
		//PORTA = 0x00;
		break;
		
		case move_f:
			Forward();
			lcount++;
			//PORTA = 0x01;
		break;
		
		case read:
			Stop();
			lcount++;
			//PORTA = 0x02;
		break;
		
		case waitG:
		//PORTA = 0x03;
		break;
		
		case rev:
			Reverse();
			lcount++;
			//PORTA = 0x04;
		break;
		
		case moveleft:
			Left();
			lcount++;
			//PORTA = 0x05;
		break;
		
		case found_dir:
			Stop();
			lat = 0x01;
			//PORTA = 0x06;
		break;
	}
}


int sonar = 0x00; /// flags for each sensor
int clear = 0x00; // flags to tell if the left or right sensor is more clear
int count = 0x00;
int latTime = 0x00;
int Fail = 0x00;
int radius = 0x00;
enum States{init, movePosLat, fail, moveLeft, forward2, turnRight, moveForward, moveRight, forward3, turnLeft, Back, turn180, reverse, wait,turnRight2, turnLeft2}state;
void Object()
{
	sonar = PIND & 0x07;
	clear = PIND & 0x18;
	//PORTA = sonar;
	//PORTC = sonar;
	switch(state)// Transitions
	{
		case init:
			state = movePosLat; 
		break;
	
		case movePosLat:
			if(Fail >= 0x02)
			{
				state = fail;
			}
			else if((sonar == 0x02) || (sonar == 0x06))
			{
				state = moveLeft;
			}
			else if(sonar == 0x03)
			{
				state = moveRight;	
			}
			else if(sonar == 0x07)
			{
				state = reverse;
			}
			else
			{
				state = moveForward;
			}
		break;
		
		case fail:
			state = fail;
		break;
		
		case moveLeft:
			if(count != 0x11)
			{
				state = moveLeft;
			}
			else
			{
				state = forward2;
				count = 0x00;
			}
		break;
		
		case forward2:
			if(sonar == 0x06)
			{
				state = turn180;
				radius = 0x01;
				Fail++;
			}
			else if(sonar == 0x07)
			{
				state = Back;
				radius = 0x01;
				Fail++;
			}
			else if((sonar == 0x04) || (sonar == 0x05))
			{
				state = forward2;
			}
			else
			{
				state = turnRight; 
				count = 0x00;
			}
		break;
		
		case turnRight:
			if(count != 0x11)
			{
				state = turnRight;
			}
			else
			{
				state = movePosLat;
				count = 0x00;
			}
		break;
		
		case moveForward:
			if((sonar == 0x00) || (sonar == 0x01) || (sonar == 0x04) || (sonar == 0x05))
			{
				state = moveForward;
			}
			else
			{
				state = movePosLat;
			}
		break;
		
		case moveRight:
			if(count != 0x11)
			{
				state = moveRight;
			}
			else
			{
				state = forward3;
			}
		break;	
			
		case forward3:
			if(sonar == 0x03)
			{
				state = turn180;
				radius = 0x00;
				Fail++;
			}
			else if(sonar == 0x07)
			{
				state = Back;
				radius = 0x00;
				Fail++;
			}
			else if((sonar == 0x01) || (sonar == 0x05))
			{
				state = forward3;
			}
			else
			{
				state = turnLeft;
				count = 0x00;
			}
		break;
		
		case turnLeft:
			if(count != 0x11)
			{
				state = turnLeft;
			}
			else
			{
				state = movePosLat;
				count = 0x00;
			}
		break;
		
		case Back:
			if(sonar != 0x00)
			{
				state = Back;
			}
			else
			{
				state = turn180;
			}
		break;
		
		case turn180:
			if(count != 0x22)
			{
				state = turn180;
			}
			else
			{
				state = movePosLat;
			}
		break;
		
		case reverse:
			if(sonar != 0x00)
			{
				state = reverse;
			}
			else
			{
				state = wait;
			}
		break;
		
		case wait:
			if(count != 0x11)
			{
				state = wait;
			}
			else if(clear == 0x01)
			{
				state = turnLeft2;
				count = 0x00;
			}
			else if(clear == 0x02)
			{
				state = turnRight2;
				count = 0x00;
			}
		break;
		
		case turnLeft2:
			if(count != 0x11)
			{
				state = turnLeft2;
			}
			else
			{
				state = forward2;
				count = 0x00;
			}
		break;
		
		case turnRight2:
			if(count != 0x11)
			{
				state = turnRight2;
			}
			else
			{
				state = forward3;
			}
		break;						
		default:
			state = init;
		break;
	}
	
	switch(state)//Actions
	{
		case init:
			
		break;
		
		case movePosLat:
		break;
		
		case fail:
		break;
		
		case moveLeft:
			Left();
			count++;
		break;
		
		case forward2:
			Forward();
		break;
		
		case turnRight:
			Right();
			count++;
		break;
		
		case moveForward:
			Forward();
		break;
		
		case moveRight:
			Right();
			count++;
		break;
		
		case forward3:
			Forward();
		break;
		
		case turnLeft:
			Left();
			count++;
		break;
		
		case Back:
			Reverse();
		break;
		
		case turn180:
			if(radius == 0x01)
			{
				Right();
			}
			else
			{
				Left();
			}
			count++;
		break;
		
		case reverse:
			Reverse();
		break;
		
		case wait:
			Stop();
			count++;
		break;
		
		case turnLeft2:
			Left();
			count++;
		break;
		
		case turnRight2:
			Right();
			count++;
		break;
	}
	
}


int main(void)
{
    /* Replace with your application code */
	//int temp = 0;
	//DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00; // outputs
	DDRC = 0x00; PORTC = 0xFF; //inputs
	DDRD = 0x00; PORTD = 0xFF;
	LState = start;
	state = init;
	TimerSet(100);
	TimerOn();
    while(1) 
    {
		LatDir();
		while(!TimerFlag);
		TimerFlag = 0;
	
    }
}


