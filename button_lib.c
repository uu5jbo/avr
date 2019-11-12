//#######################################################################################################################
//#
//#    ФУНКЦИИ РАБОТЫ С КЛАВОЙ
//#
//# в начале проинитить библиотеку вызовом BtnInit();
//# организовать вызов из прерывания с частотой 100Гц - функцию BtnExe();
//# чтение значения состояния флагов кнопок производится с помощью функции BtnGet();
//# например так: 
//# char BtnMask = BtnGet();
//# if (BtnMask & BTN_SHRT_UP)      {....ветка короткого нажатия кнопки UP}
//# if (BtnMask & BTN_SHRT_DN)      {....ветка короткого нажатия кнопки DN}
//# if (BtnMask & BTN_LONG_RIGHT)   {....ветка длинного нажатия кнопки RIGHT}
//# и т.д.
//#
//#######################################################################################################################

//настройка параметров работы функций
#define BTN_LOCK_TIME         30                /*время обработки дребезга в милисекундах (10-100)*/
#define BTN_LONG_TIME        1000            /*время фиксации длинного нажатия в милисекундах (1000 - 2500)*/

//настройки портов
#define BTN_PORT        PORTC                    /*порт чтения кнопок*/
#define BTN_DDR            DDRC
#define BTN_PIN         PINC                    
    #define BTN_LINE_UP     (1<<0)                /*пины чтения кнопок*/
    #define BTN_LINE_DN     (1<<1)
    #define BTN_LINE_LEFT    (1<<2)
    #define BTN_LINE_RIGHT    (1<<3)
    
//глобальные переменные
volatile uint8_t BtnFlags;                        //байт флагов нажатия кнопки
    #define BTN_SHRT_UP        (1<<0)                /*бит короткого нажатия кнопки up*/ 
    #define BTN_SHRT_DN        (1<<1)                /*бит короткого нажатия кнопки dn*/ 
    #define BTN_SHRT_LEFT    (1<<2)                /*бит короткого нажатия кнопки left*/ 
    #define BTN_SHRT_RIGHT    (1<<3)                /*бит короткого нажатия кнопки right*/ 
    #define BTN_LONG_UP        (1<<4)                /*бит длинного нажатия кнопки up*/ 
    #define BTN_LONG_DN        (1<<5)                /*бит длинного нажатия кнопки dn*/ 
    #define BTN_LONG_LEFT    (1<<6)                /*бит длинного нажатия кнопки left*/ 
    #define BTN_LONG_RIGHT    (1<<7)                /*бит длинного нажатия кнопки right*/ 


//#######################################################################################################################
//функция настройки библиотеки работы с кнопками
void BtnInit (void)
{    
    BTN_DDR &= ~(BTN_LINE_UP| BTN_LINE_DN| BTN_LINE_LEFT| BTN_LINE_RIGHT);    //на ввод
    BTN_PORT |= (BTN_LINE_UP| BTN_LINE_DN| BTN_LINE_LEFT| BTN_LINE_RIGHT);    //подтяжка вкл
}

//----------
//функция чтения данных о нажатии кнопок
char BtnGet (void)
{
    cli ();
    char temp = BtnFlags;
    BtnFlags = 0;
    sei ();
    return temp;
}

//----------
//ФУНКЦИЯ ОБРАБОТКИ НАЖАТИЙ КЛАВИШ (вызывать в прерывании с частотой 100 Гц)
//короткое нажатие устанавливает бит BTN_SHRT_X глобальной переменной BtnFlags
//длинное нажатие устанавливает бит BTN_LONG_X глобальной переменной BtnFlags
void BtnExe (void)
{    
    static unsigned char BtnLockBit;            //ащелка (защита от дребезга)
    static unsigned char BtnLockCoun;            //счетчик защелки (защита от дребезга)
    static unsigned char BtnLongCoun;            //счетчик длинного нажатия
    static unsigned char BtnLastState;            //последнее состояние кнопок перед отпусканием

    char mask = 0;
    if (! (BTN_PIN & BTN_LINE_UP))        mask = BTN_SHRT_UP;
    if (! (BTN_PIN & BTN_LINE_DN))        mask = BTN_SHRT_DN;
    if (! (BTN_PIN & BTN_LINE_LEFT))    mask = BTN_SHRT_LEFT;
    if (! (BTN_PIN & BTN_LINE_RIGHT))    mask = BTN_SHRT_RIGHT;
    
    if (mask){                                    //опрос состояния кнопки
        if (BtnLockCoun < (BTN_LOCK_TIME/10)){    //клавиша нажата
            BtnLockCoun++;
            return;                                //защелка еще не дощитала - возврат
        }
        BtnLastState = mask;
        BtnLockBit =1;                            //нажатие зафиксировано                
        if (BtnLongCoun >= (BTN_LONG_TIME/10))                                
            return;                                //возврат, т.к. счетчик длинн нажат досчитал до максимума еще раньше        
        if (++BtnLongCoun >= (BTN_LONG_TIME/10))
            BtnFlags |= (BtnLastState<<4);        //счетчик досчитал до максимума - устанавливаем биты длинного нажатия 
    }            
    else{                                        //клавиша отжата            
        if (BtnLockCoun){
            BtnLockCoun --;
            return;                                //защелка еще не обнулилась - возврат
        }
        if (! BtnLockBit)                        //СТАТИЧЕСКИЙ ВОЗВРАТ
            return;                                
        BtnLockBit =0;                            //отжатие зафиксировано
        if (BtnLongCoun < (BTN_LONG_TIME/10))
            BtnFlags |= BtnLastState;            //установка бита короткого нажатия
        BtnLongCoun = 0;                        //сброс счетчика длительности нажатия
    }
}    

//######################################################################################################################