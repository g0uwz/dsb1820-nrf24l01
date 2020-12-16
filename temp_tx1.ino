


/*
  Wireless temperature transmitter.
  orignal code
  https://www.elec-cafe.com/nrf24l01-wireless-temperature-monitoring-ds18b20-arduino-uno/?fbclid=IwAR2yrgHGsi6RM3-ygCthsXPAvTPyHcCiBUyCoY6r9XV_xnp57tSVWHknwSA
  

  DS18B20 Temperature SensorWire
  BLACK <-> ARDUINO GND
  RED <-> ARDUINO 5V
  YELLOW <-> ARDUINO D2 - requires to be pulled high to 5v with 4k7 resistor

  nRF24L01+ to Arduino Nano
  GND <–> GND
  VCC <–> Arduino (3.3V) not 5v
  CE <–> Arduino pin 7
  CSN <–> Arduino pin 8
  SCK <–> Arduino pin 13
  MOSI <–> Arduino pin 11
  MISO <–> Arduino pin 12
  Connect 10uf 16v electrolytic cap as close as possible to nrf24 power supply connections.
  */

  
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float tempCelsius;    // temperature in Celsius
float tempFahrenheit; // temperature in Fahrenheit
float temp1;

RF24 radio(7, 8);   // CE, CSN

const uint64_t pipes[1] = { 0xF0F0F0F0E1LL };  

void setup(void) {
  Serial.begin(9600);
  sensors.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);      // Set the transmit power MIN.LOW,HIGH,MAX
  radio.setDataRate(RF24_250KBPS);   // Set data rate 250KBPS  1MBPS  2MBPS
  radio.setChannel(124); // Use a channel unlikely to be used by Wifi, Microwave ovens etc
  radio.openWritingPipe(pipes[1]);
  radio.stopListening();
}

void loop(void)
{
  sensors.requestTemperatures();
  tempCelsius = sensors.getTempCByIndex(0);  // read temperature in Celsius
  tempFahrenheit = tempCelsius * 9 / 5 + 32; // convert Celsius to Fahrenheit
  Serial.print(" Outdoor Station: ");
  Serial.print(tempCelsius);    // print the temperature in Celsius
  Serial.print("°C");
  Serial.print("  -  ");        // separator between Celsius and Fahrenheit
  Serial.print(tempFahrenheit); // print the temperature in Fahrenheit
  Serial.println("°F");
  temp1 = sensors.getTempCByIndex(0);
  radio.write(&temp1, sizeof(temp1));
  delay(1000);
}
