
/*
  Wireless temperature transmitter & receiver
  orignal code
  https://www.elec-cafe.com/nrf24l01-wireless-temperature-monitoring-ds18b20-arduino-uno/?fbclid=IwAR2yrgHGsi6RM3-ygCthsXPAvTPyHcCiBUyCoY6r9XV_xnp57tSVWHknwSA
  I have modified code for LCD display with I2C.

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
  Connect 10uf 16v electrolytic cap as close as possible to nrf power supply connections.

  I2C LCD display 16×2  (PCF 8574T CONTROLLER) HEX adress (0x27)
  SDA <-> Arduino A4
  SCL <-> Arduino A5
  VCC <-> Arduino 5V (Requires 25/30ma)
  GND <-> Arduino GND

*/


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <OneWire.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display


float temp1;

RF24 radio(7, 8); // CE, CSN

const uint64_t pipes[1] = { 0xF0F0F0F0E1LL };

void setup(void) {
  Serial.begin(9600);
  radio.begin();

  radio.setPALevel(RF24_PA_MIN); // Set the transmit power MIN.LOW,HIGH,MAX. Not required for receiver (BUT SET TO MIN ANYWAY)
  radio.setDataRate(RF24_250KBPS);  // Set data rate 250KBPS  1MBPS  2MBPS - 250KBPS FOR BEST SENSITIVITY
  radio.setChannel(124);
  radio.openReadingPipe(1, pipes[1]);

  radio.startListening();      //Stop the radio from transmitting -  receive mode
  lcd.init();                 // initialize the lcd

  lcd.backlight();           // backlight on
  lcd.clear();

  lcd.print("Booting nRF24L01+");
  delay(500);
  lcd.clear();
  lcd.print("Connecting.....");
  delay(500);
}

void loop(void)
{
  // lcd.clear();             // optional - enabling will clear and refresh data, disabling will keep data on lcd static.

  delay(100);
  if ( radio.available() )
  {
    delay(500);

    radio.read(&temp1, sizeof(temp1));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Outdoor Station");

    lcd.setCursor(0, 1);
    lcd.print("Probe 1");
    lcd.setCursor(9, 1);
    lcd.print(temp1);
    lcd.print("C");

    delay(500);




  }
  else
  {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("No Radio");
    lcd.setCursor(4, 1);
    lcd.print("available");

  }
  delay(1000);
}
