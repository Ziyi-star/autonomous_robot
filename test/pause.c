int main(void) {
    setup_heartbeat_timer();
    USART_init(UBRR_SETTING);

    unsigned char some_letter = 'A' - 1;

    while (1) {
        if (!paused) {
            OR_HEARTBEAT &= ~(1 << OP_HEARTBEAT);
            _delay_ms(500);
            OR_HEARTBEAT |= (1 << OP_HEARTBEAT);
            _delay_ms(500);
            
            some_letter++;
            USART_transmitByte(some_letter);
            USART_transmitByte('\n');
            if (some_letter == 'Z') {
                some_letter = 'A' - 1;
            }
        } else {
            // Pause mode: Run LED sequence and send pause message
            run_led_sequence(&regmdl);
        }

        // Check for 'P' character to toggle pause state
        if (USART_available()) {
            char input = USART_receive();
            if (input == 'P') {
                paused = !paused; // Toggle paused state
                if (!paused) {
                    cnt = 0; // Reset counter when unpausing
                    clear(&regmdl); // Clear LED state when unpausing
                }
            }
        }
    }

    return 0;
}
