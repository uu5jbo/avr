#ifndef PERIPF_INI_H_INCLUDED
#define PERIPF_INI_H_INCLUDED

void timer_init(void);
void Reset(void);
ISR(TIMER1_OVF_vect);

#endif // PERIPF_INI_H_INCLUDED
