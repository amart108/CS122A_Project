/*Partner(s)Name & email: Antonio Martinez, amart108@ucr.edu
 *Assignment
 *Exercise Description: 122A final project movement test and crash prevention test
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

void Back()
{
	//Moving the car straight backwards
	//B = 9: Move backwards
	PORTB = 0x09;	
}


int temp = 0x00;
int count = 0x00;
enum States{init, forward, left, right, surround, stop}state;
void tick()
{
	temp = PIND & 0x07;
	PORTA = 0x00;
	//PORTC = temp;
	switch(state)// Transitions
	{
		case init:
			state = forward;  
		break;
		
		case forward:
			
			if(temp == 0x03)
			{
				state = right;
				//count = 0;
			}
			else if((temp == 0x06) || (temp == 0x02))
			{
				state = left;
				//count = 0;
			}
			else if((temp == 0x07))
			{
				state = surround;
				//	count = 0;
			}
			else
			{
				state = forward;
			}
		break;
		
		case left:
			if((count != 0x11))
			{
				state = left;
			}
			else
			{
				state = stop;
				count = 0x00;
			}
		break;
		
		case right:
			if((count != 0x11))
			{
				state = right;
			}
			else
			{
				state = stop;
				count = 0x00;
			}	
		break;

		case surround:
			if((count != 0x14))
			{
				state = surround;
			}
			else
			{
				state = stop;
				count = 0x00;
			}
		break;
		
		case stop:
			if((count != 0x14))
			{
				state = stop;
			}
			else
			{
				state = forward;
				count = 0x00;
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
		
		case forward:
			Forward();
			//PORTA = 0x01;
		break;
			
		case left:
			Left();
			//PORTA = 0x03;
			count++;
		break;		
		
		case right:
			Right();
			count++;
			//PORTA = 0x04;
		break;
		
		case surround:
			Back();
			count++;
			//PORTA = 0x05;
		break;
		
		case stop:
			Stop();
			//PORTA = 0x06;
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
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0x00; PORTD = 0xFF;
	state = init;
	TimerSet(100);
	TimerOn();
    while(1) 
    {
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

