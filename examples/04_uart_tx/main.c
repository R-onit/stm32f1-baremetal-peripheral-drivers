#include "stm32f103xb.h"
#include "uart.h"


int main(void){
	uar2_rxtx_init();

	while(1){
		char recieved = USART2_read();
		USART2_write(recieved);

	}
}