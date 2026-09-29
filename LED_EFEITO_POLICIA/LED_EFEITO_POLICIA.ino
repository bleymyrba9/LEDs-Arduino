#include <Arduino.h>
#include <FastLED.h>
#define NUM_LEDS       12
#define LED_DATA_PIN   18
uint8_t brightnessValue = 180;
unsigned long lastEffectTime = 0;
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  delay(1000);
  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.show();
}

void loop() {
  static uint8_t step = 0;
  static bool flip = false;
  
  // Define o tempo de cada passo: 200ms para a parte pausada, 80ms para as piscas rápidas
  unsigned long currentInterval = (step < 8) ? 200 : 80;

  // Temporizador baseado no intervalo dinâmico
  if (millis() - lastEffectTime < currentInterval) return;
  lastEffectTime = millis();

  FastLED.setBrightness(brightnessValue);

  // FASE 1: Sequência Pausada (Passos 0 a 7)
  if (step < 8) {
    fill_solid(leds, NUM_LEDS, CRGB::Black); // Apaga tudo na fase pausada
    for(int8_t i = 0;i<=2;i++){
    switch (step % 4) {
      case 0: // Metade Vermelho
        fill_solid(leds, NUM_LEDS / 2, CRGB::Red);
        break;
      case 1: // Apagado
        break;
      case 2: // Metade Azul
        fill_solid(leds + (NUM_LEDS / 2), NUM_LEDS / 2, CRGB::Blue);
        break;
      case 3: // Apagado
        break;
      }
    }
  } 
  // FASE 2: Pisca Rápido Sem Pausa (Passos 8 a 15 = 4 trocas completas)
  else {
    flip = !flip;
    fill_solid(leds, NUM_LEDS / 2, flip ? CRGB::Red : CRGB::Blue);
    fill_solid(leds + (NUM_LEDS / 2), NUM_LEDS / 2, flip ? CRGB::Blue : CRGB::Red);
  }

  FastLED.show();

  // Avança para o próximo passo e reinicia após o passo 15
  step = (step + 1) % 16;
}
