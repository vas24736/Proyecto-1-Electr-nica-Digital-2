# Proyecto-1-Electr-nica-Digital-2
# Sistema de Monitoreo para Cadena de Frío

Proyecto desarrollado para el curso **Electrónica Digital 2 (BE3029)**. El sistema consiste en un prototipo de monitoreo de temperatura para una cadena de frío, implementado mediante un **ESP32**, un sensor de temperatura **LM35**, un **LED RGB**, un **servomotor**, tres **displays de 7 segmentos** y comunicación Wi-Fi con **Adafruit IO**.

## Descripción

El sistema permite realizar inspecciones puntuales de temperatura mediante un pushbutton. Al presionar el botón, el ESP32 adquiere la señal analógica proporcionada por el sensor LM35 y la convierte a un valor de temperatura en grados Celsius.

A partir de la temperatura obtenida, el programa determina el estado térmico del sistema y controla un LED RGB y un servomotor. La temperatura también se muestra con un decimal mediante tres displays de 7 segmentos utilizando multiplexeo.

Finalmente, el ESP32 se conecta mediante Wi-Fi a Adafruit IO para transmitir y visualizar remotamente las mediciones de temperatura.

## Funcionamiento

El proceso general del sistema es el siguiente:

1. El ESP32 inicializa los periféricos, interrupciones, temporizadores, PWM y la conexión con Adafruit IO.
2. El sistema espera la activación del pushbutton.
3. Al presionar el botón, una interrupción activa la solicitud de una nueva medición.
4. Se adquiere la señal analógica del LM35.
5. La lectura ADC se convierte a temperatura en grados Celsius.
6. La temperatura se separa en decenas, unidades y decimal.
7. Los tres displays muestran la última temperatura registrada mediante multiplexeo.
8. El LED RGB indica el estado térmico.
9. El servomotor controla la posición de la rejilla de ventilación.
10. La temperatura registrada se envía a Adafruit IO.

## Rangos de temperatura

| Temperatura | Estado | LED RGB | Rejilla |
|---|---|---|---|
| 0 < T ≤ 23 °C | Riesgo por frío | Azul | Cerrada (0°) |
| 23 < T ≤ 25 °C | Rango seguro | Verde | Media (45°) |
| 25 < T ≤ 27 °C | Cerca del límite | Amarillo | Media (45°) |
| T > 27 °C | Riesgo por calor | Rojo | Abierta (90°) |

## Componentes utilizados

- ESP32
- Sensor de temperatura LM35
- Pushbutton
- LED RGB de cátodo común
- Servomotor
- 3 displays de 7 segmentos de cátodo común
- 3 transistores 2N700 para el multiplexeo
- Resistencias de 220 Ω para los displays
- Resistencias adicionales para el circuito
- Protoboard
- Jumpers
- Conexión Wi-Fi
- Adafruit IO

## Distribución de pines

| Componente | GPIO |
|---|---|
| Pushbutton | GPIO 34 |
| LM35 | A7 |
| LED RGB - Rojo | GPIO 4 |
| LED RGB - Azul | GPIO 17 |
| LED RGB - Verde | GPIO 16 |
| Servomotor | GPIO 13 |
| Display 1 | GPIO 19 |
| Display 2 | GPIO 18 |
| Display 3 | GPIO 5 |

Los segmentos de los displays se encuentran conectados a diferentes GPIO del ESP32 y son compartidos entre los tres displays para implementar el multiplexeo.

## Conversión de temperatura

El ADC del ESP32 fue configurado con una resolución de 12 bits. La temperatura se obtiene mediante:

```cpp
temperatura = analogRead(T);
temp = (temperatura * 330) / 4095;
