#include<Wire.h>
#include <SoftwareSerial.h>

const int MPU=0x68;  // I2C address of the MPU-6050
unsigned long Time;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
float to_g_force;
const int8_t AFS_SEL_2G   = 0x00;
const int8_t AFS_SEL_4G   = 0x08;
const int8_t AFS_SEL_8G   = 0x10;
const int8_t AFS_SEL_16G  = 0x18;
SoftwareSerial BTSerial(2, 3); // RX | TX


void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  BTSerial.begin(9600); 
  int8_t afs_sel = AFS_SEL_2G;
   Wire.write(0x1C);
   Wire.write(afs_sel);
   to_g_force = 1. / (16384 >> (afs_sel >> 3 & 0x03));

// calibrate the accelerometer
  const int8_t XG_OFFS_USRH = 0x06;
  const int16_t offset_x = -1135;
  const int16_t offset_y =   -224;
  const int16_t offset_z =  1515;
  Wire.beginTransmission(0b1101000);
  Wire.write(XG_OFFS_USRH);

  uint8_t offsets[6];
  offsets[0] = static_cast<uint8_t>(offset_x >> 8);
  offsets[1] = static_cast<uint8_t>(offset_x & 0x00ff);
  offsets[2] = static_cast<uint8_t>(offset_y >> 8);
  offsets[3] = static_cast<uint8_t>(offset_y & 0x00ff);
  offsets[4] = static_cast<uint8_t>(offset_z >> 8);
  offsets[5] = static_cast<uint8_t>(offset_z & 0x00ff);
  Wire.write(offsets, 6);
  Wire.endTransmission();


  const int8_t XG_OFFS_USRH1 = 0x13;
  const int16_t offset_x1 = 61;
  const int16_t offset_y1 =   -60;
  const int16_t offset_z1 =  -4;
  Wire.beginTransmission(0b1101000);
  Wire.write(XG_OFFS_USRH1);

  uint8_t offsets1[6];
  offsets1[0] = static_cast<uint8_t>(offset_x1 >> 8);
  offsets1[1] = static_cast<uint8_t>(offset_x1 & 0x00ff);
  offsets1[2] = static_cast<uint8_t>(offset_y1 >> 8);
  offsets1[3] = static_cast<uint8_t>(offset_y1 & 0x00ff);
  offsets1[4] = static_cast<uint8_t>(offset_z1 >> 8);
  offsets1[5] = static_cast<uint8_t>(offset_z1 & 0x00ff);
  Wire.write(offsets1, 6);
  Wire.endTransmission();
}
void loop(){
  
 // Time=millis();
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  float AcXx = AcX * to_g_force;
  float AcYy = AcY * to_g_force;
  float AcZz = AcZ * to_g_force;

  
  BTSerial.print(AcXx);BTSerial.print("|");
  BTSerial.print(AcYy);BTSerial.print("|");
  BTSerial.print(AcZz);BTSerial.print(",");
  
  BTSerial.print(Tmp/340.00+36.53); BTSerial.print(","); 
  
  BTSerial.print(GyX/131.0);BTSerial.print("|");
  BTSerial.print(GyY/131.0);BTSerial.print("|");
  BTSerial.print(GyZ/131.0);BTSerial.print(";");

  BTSerial.print("123");BTSerial.print(";");
  BTSerial.println("1234");
  
  
  
  delay(1);
}
