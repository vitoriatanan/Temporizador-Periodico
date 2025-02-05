#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos pinos onde os LEDs estão conectados
#define PIN_LED_RED 11
#define PIN_LED_YELLOW 12
#define PIN_LED_GREEN 13

// Intervalo de tempo (em milissegundos) para alterar os estados do semáforo
#define INTERVALO_SEMAFORO 3000
// Intervalo de tempo para mensagens pela porta serial
#define INTERVALO_SERIAL 1000

// Variáveis globais para controle do semáforo
int estado_atual = 0;

// Função que controla o semáforo
bool controle_semaforo(struct repeating_timer *t) {
    switch (estado_atual) {
        case 0: // Estado Vermelho
            gpio_put(PIN_LED_RED, 1);
            gpio_put(PIN_LED_YELLOW, 0);
            gpio_put(PIN_LED_GREEN, 0);
            printf("LED Vermelho...\n");  // Envia mensagem na serial
            estado_atual = 1; // Próximo estado: Amarelo
            break;
        case 1: // Estado Amarelo
            gpio_put(PIN_LED_RED, 0);
            gpio_put(PIN_LED_YELLOW, 1);
            gpio_put(PIN_LED_GREEN, 0);
            printf("LED Amarelo...\n");
            estado_atual = 2; // Próximo estado: Verde
            break;
        case 2: // Estado Verde
            gpio_put(PIN_LED_RED, 0);
            gpio_put(PIN_LED_YELLOW, 0);
            gpio_put(PIN_LED_GREEN, 1);
            printf("LED Verde...\n");
            estado_atual = 0; // Próximo estado: Vermelho
            break;
    }
    return true; // Continua repetindo o timer
}

int main() {
    // Inicialização dos pinos
    gpio_init(PIN_LED_RED);
    gpio_init(PIN_LED_YELLOW);
    gpio_init(PIN_LED_GREEN);

    gpio_set_dir(PIN_LED_RED, GPIO_OUT);
    gpio_set_dir(PIN_LED_YELLOW, GPIO_OUT);
    gpio_set_dir(PIN_LED_GREEN, GPIO_OUT);

    // Inicialização da porta serial
    stdio_init_all();

    // Configuração do timer repetitivo para o semáforo
    struct repeating_timer timer_semaforo;
    add_repeating_timer_ms(-INTERVALO_SEMAFORO, controle_semaforo, NULL, &timer_semaforo);


    // Loop principal
    while (1) {
        tight_loop_contents(); // Mantém o programa ativo
    }

    return 0;
}
