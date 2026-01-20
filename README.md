# Read-Temp-Humid
Set EC25 configuration; set sensor address and baud rate.
## Objective 
* เพื่อเรียนรู้การเชื่อมต่อเซนเซอร์ **Modbus RTU (RS485)**
* เพื่อฝึกการสร้างพอร์ตสื่อสาร **(SERCOM)** บนบอร์ด SAMD
* เพื่อจัดการข้อมูล (Data Parsing) และตั้งค่า Hardware สำหรับโมดูล **EC25**

## Files 
* **`code_5_intern.ino`** : Source Code หลักควบคุมการทำงาน
* **`README.md`** : เอกสารอธิบายรายละเอียดงาน

## Assignment 
### 1. Hardware Configuration
* **Code Section:** `setup()` 
* **Task:** กำหนด Logic Pin (A1, A3, A5) เพื่อเปิดใช้งานโมดูล **Quectel EC25** และปิดโมดูล Neoway เพื่อลดสัญญาณกวน

### 2. Communication Interface
* **Code Section:** `Uart modbus_iso` & `SERCOM1_Handler`
* **Task:** สร้าง Serial Port ชุดใหม่โดยใช้ **SERCOM1** บน Pin 8 (TX) และ Pin 9 (RX) เพื่อแยกช่องทางสื่อสาร Modbus ออกจาก Serial ปกติ

### 3. Data Processing
* **Code Section:** `loop()`
* **Task:** อ่านค่า Register จากเซนเซอร์และแปลงหน่วย
    * **Bit Shifting:** รวมค่า High/Low Byte สำหรับค่าแสง (Lux)
    * **Unit Conversion:** หารทศนิยมสำหรับค่าอุณหภูมิ, ลม, ความชื้น ให้ถูกต้องตาม Datasheet
