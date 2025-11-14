#include "uart.h"

int _write(int file, char *ptr, int length )
{
    for (int i=0 ;i< length ;i++)
    {
        USART2_write(*ptr++);
    }
    return length;
}