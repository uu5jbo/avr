#include <avr/io.h> //Библиотека ввода/вывода
#include <avr/interrupt.h> //Библиотека прерываний
 
unsigned char status=0;
 
void pause (unsigned int a);
void init_timer (void);
void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data );
 
 
//Обработчик прерываний по срабатываю таймера 0
ISR (TIMER0_OVF_vect)
{
	if ((PINC&0x01)==0x00)
	{
//Обработка нажатия кнопки
pause(1000); //Пауза на 0.01С
if ((PINC&0x01)==0x00) //Если нажата кнопка
	{
while ((PINC&0x01)==0x00); //Пока нажата кнопка
if (status==0) 
status=1; //Статус становится 1
	}
	}
else 
 
if ((PINC&0x02)==0x00)
	{
pause(1000); //Пауза на 0.01С
if ((PINC&0x02)==0x00) //Если нажата кнопка
	{
while ((PINC&0x02)==0x00); //Пока нажата кнопка
if (status==0) 
status=2; //Статус становится 1
	}
	}
else 
 
if ((PINC&0x04)==0x00)
	{
pause(1000);
if ((PINC&0x04)==0x00) 
	{
while ((PINC&0x04)==0x00); 
if (status==0) 
status=3; 
	}
	}
else 
 
if ((PINC&0x08)==0x00)
	{
pause(1000); 
if ((PINC&0x08)==0x00)
	{	
while ((PINC&0x08)==0x00); 
if (status==0) 
status=4; 
	}
	}
else 
 
status=0; //В другом случаи ни одна кнопка не нажата
TCNT0=0x00; //Обнуляем таймер счетчик 0
TIFR=0x00; //Продолжаем ожидать нажатия
return;
}
 
//Программа задержки
void pause (unsigned int a)
{
unsigned int i; 
for (i=a;i>0;i--); 
}
 
//Программа иницализации Таймера 0 0
void init_timer (void)
{
TIMSK=(1<<TOIE0); 
TCCR0=(1<<CS00)|(1<<CS01)|(0<<CS02); //Делитель тактовой частоты на 64
}
 
void USART_Init( unsigned int ubrr)
{
/* Set baud rate */	
UBRRH = (unsigned char)(ubrr>>8);	
UBRRL = (unsigned char)ubrr;
/* Enable receiver and transmitter */
UCSRB=(1<<RXEN)|(1<<TXEN);
UCSRB |= (1 << RXCIE);
 
//UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
/* Set frame format: 8data, 2stop bit */
UCSRC=0x86;//	
UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}
 
//ПРограмма передачи данных по UART
void USART_Transmit( unsigned char data ) 
{
	while ( !(UCSRA & (1<<UDRE)) ); 
	UDR = data; 		        
}
 
//Программа определения и вывода нажатой кнопки
void ifstatus(void)
{
if (status == 1)
{USART_Transmit('1');
USART_Transmit(0x0d);
status=0;}
else 
if (status == 2)
{USART_Transmit('2');
USART_Transmit(0x0d);
status=0;}
 
if (status == 3)
{USART_Transmit('3');
USART_Transmit(0x0d);
status=0;}
 
if (status == 4)
{USART_Transmit('4');
USART_Transmit(0x0d);
status=0;}
 
}
 
int main(void)
{
	DDRC=0x00; //Указываем направление работы порта С
	PORTC=0xFF; //Включаем подтягивающие резисторы
	init_timer(); //Инициализация Таймера 0 0
 
	USART_Init (3);//115200 8MHz
 
	USART_Transmit('O');
	USART_Transmit('k');
	USART_Transmit('!');
 
	USART_Transmit(0x0d);
	USART_Transmit(0x0a);
 
sei(); //Разрешаем глобально прерывания
 
	while(1) //Вечный цикл
	{
	cli(); //Запрещаем прерывания
	ifstatus(); //Проверяем какая кнопка была нажата
 
	sei(); //Разрешаем опять прерывания
	pause(1000); //Пауза
	}
return 1;
}