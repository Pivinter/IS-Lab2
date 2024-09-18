#include <avr/io.h>
#include <util/delay.h>

#define BTN_PIN PINB 
#define BTN PB1         

uint8_t effect = 0;
uint8_t effect_running = 0;
uint16_t press_duration = 0;
uint8_t button_pressed = 0; 

void handle_button(void) {
    if (!(BTN_PIN & (1 << BTN))) {
        press_duration++;
        button_pressed = 1;
    } else if (button_pressed) {
        if (press_duration > 200) {
            if (!effect_running) {
                effect = (effect + 1) % 3;
            }
        } else {
            effect_running = !effect_running;
        }
        press_duration = 0;
        button_pressed = 0;
    }
}
void running_fire(void) {
    static uint8_t d = 0x01;
    d <<= 1;
    if (d == 0) d = 0x01;
    PORTD = d;
    _delay_ms(100);
}
void running_shadow(void) {
    static uint8_t d = 0x80;
    d >>= 1;
    if (d == 0) d = 0x80;
    PORTD = d;
    _delay_ms(100);
}
void johnson_counter(void) {
    static uint8_t johnson = 0x01;
    johnson = ~johnson;
    PORTD = johnson;
    _delay_ms(100);
}
int main(void) {
    DDRD = 0xFF;  
    DDRB &= ~(1 << BTN); 
    PORTB |= (1 << BTN); 
    while (1) {
        handle_button();
        if (effect_running) {
            switch (effect) {
                case 0:
                    running_fire();
                    break;
                case 1:
                    running_shadow();
                    break;
                case 2:
                    johnson_counter();
                    break;
            }
        } else {
            _delay_ms(10);
        }
    }
    return 0;
}
