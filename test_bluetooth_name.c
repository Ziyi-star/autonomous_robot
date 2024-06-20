#include <avr/io.h>
#include "iesusart.h"

int main(void) {
	USART_init(UBRR_SETTING);

	while(1){
    USART_print("Ziyi LIU!\n");
    }
    return 0;

}
