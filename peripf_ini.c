
void Reset()
{
    // Outs PORTC
    DDRC    |= 1<<PC5|1<<PC4|1<<PC3|1<<PC2; //Direction(Out)
    PORTC   |=  0x00;                       // State(Low)
    //LEDs PORTB
    DDRB    |= 1<<PB1|1<<PB2|1<<PB3; // Direction(Out)
    PORTB   |= 0x00;                 // State(Low)
    //Buttons PORTD
    DDRD    |= 0x00;                //Direction (Input)
    PORTD   |= 1<<PD7|1<<PD6|1<<PD5; //State (High Pull-Up)
}

void timer_init(void)
{
    TCCR1B |= (1<<CS12)|(1<<CS10);  //Предделитель = 1024
    TIMSK |= (1<<TOIE1);    //Разрешить прерывание по переполнению таймера 1
    TCNT1 = 61630;          //Начальное значение таймера

}

extern uint8_t state;
uint8_t keys;
ISR(TIMER1_OVF_vect)
{
    keys = PIND;
    state = (~keys) & 0xe0;
}
