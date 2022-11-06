#ifndef INTERRUPT_H_
#define INTERRUPT_H_

void enableTimerInterrupt1(void)
{
	TIMSK1 |= (1<<OCIE1A);	 // Timer 1 compare interrupt 1 enable
}

void disableTimerInterrupt1(void)
{
	TIMSK1 &= ~(1<<OCIE1A);	 // Timer 1 compare interrupt 1 disable
}

void initTimerInterrupt1(void)
{
	TCNT1 = 0x00;		// Init counter to 0
	TCCR1B |= ((1<<WGM12)|(1<<CS12)|(1<<CS10)); //Set prescaler 0 to 1
	OCR1A = 46875 - 1;
	enableTimerInterrupt1();
}

void intInteruptPort(void){
	PORTE &= ~(1<<INT4);//set PortE4(pin 2) as input
	EIMSK |= (1<<INT4);//Enable INT4 interrupt
	EICRB |= (1<<ISC00);//Enable any edge as INT request
}

#endif	