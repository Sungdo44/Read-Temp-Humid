#include <Arduino.h>
#include "wiring_private.h"
#include <ModbusMaster.h>
#define MAX485_DE 4

ModbusMaster node;
uint8_t result;
int t[16] = {};
int i = 1;

Uart modbus_iso (&sercom1, 9, 8, SERCOM_RX_PAD_1, UART_TX_PAD_0);

void setup() {
  // เริ่มต้นตั้งค่าขา Logic
  pinMode(A1, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A6, OUTPUT); // ขาสำหรับ Neoway (ปิดไว้)

  // --- สูตรสำหรับ Quectel EC25 (ตามตาราง V3.0) ---
  digitalWrite(A1, HIGH); 
  digitalWrite(A3, LOW);  
  digitalWrite(A5, HIGH); 

  // ปิดการทำงานของโมดูลอื่น (Neoway) เพื่อกันสัญญาณกวน
  digitalWrite(A6, HIGH);

  Serial.begin(4800);

  modbus_iso.begin(9600);
  pinPeripheral(9, PIO_SERCOM);
  pinPeripheral(8, PIO_SERCOM);

  pinMode(A4, OUTPUT);    //RS485 - I2C
  digitalWrite(A4, LOW);  //LOW:RS485   HIGH:I2C

  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_DE, 0);

  node.begin(1, modbus_iso);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

}

void loop() {

  result = node.readInputRegisters(500, 16);
  Serial.println("==> result: " + String(result));
  delay(100);
  if (result == node.ku8MBSuccess)
  {
    t[0] = node.getResponseBuffer(0);
    t[1] = node.getResponseBuffer(1);
    t[2] = node.getResponseBuffer(2);
    t[3] = node.getResponseBuffer(3);
    t[4] = node.getResponseBuffer(4);
    t[5] = node.getResponseBuffer(5);
    t[6] = node.getResponseBuffer(6);
    t[7] = node.getResponseBuffer(7);
    t[8] = node.getResponseBuffer(8);
    t[9] = node.getResponseBuffer(9);
    t[10] = node.getResponseBuffer(10);//high 16bit lux
    t[11] = node.getResponseBuffer(11);//low 16bit lux
    uint32_t Lux_value = ((uint32_t)t[10] << 16) | t[11];
    t[12] = node.getResponseBuffer(12);
    t[13] = node.getResponseBuffer(13);
    t[14] = node.getResponseBuffer(14);
    t[15] = node.getResponseBuffer(15);
    Serial.println("==> Received: " + String(i));
    Serial.println("> Temperature: " + String(t[5] / 10.0) + " °C");
    Serial.println("> Humidity: " + String(t[4] / 10.0) + " %RH");
    Serial.println("> Wind_Speed: " + String(t[0] / 10.0) + "m/s");
    Serial.println("> Wind_Force: " + String(t[1] ) + " Level");
    Serial.println("> Wind_Direction: " + String(t[2] ) + "");
    Serial.println("> Wind_Direction: " + String(t[3] ) + " °");
    Serial.println("> Noise: " + String(t[6] / 10.0) + " dB");
    Serial.println("> PM2.5: " + String(t[7] ) + " µg/m³");
    Serial.println("> PM10: " + String(t[8] ) + " µg/m³");
    Serial.println("> Pressure: " + String(t[9] / 10.0) + " kPa");
    Serial.println("> Lux: " + String(Lux_value)+ " lux");
    Serial.println("> Light_value: " + String(t[12]) + " Hundred Lux");
    Serial.println("> Rainfall: " + String(t[13] / 10.0) + " mm");
    Serial.println("> Compass: " + String(t[14] / 100.0) + "");
    Serial.println("> Radiation: " + String(t[15] ) + " W/m²");
    Serial.println();
  }
  delay (10000);
  i++;

}

void preTransmission() {
  digitalWrite(MAX485_DE, 1);
}

void postTransmission() {
  digitalWrite(MAX485_DE, 0);
}

void SERCOM1_Handler()
{
  modbus_iso.IrqHandler();
}
