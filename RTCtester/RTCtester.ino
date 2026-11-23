// DS1307 & TM1637 数码管 —— 实验2 修改版
// 修改1：TM1637 显示"组号:分钟"（前两位组号，后两位分钟，中间冒号常亮）
// 修改2：串口输出时间数据前，先输出本组组号
// 接线：DS1307: VCC→5V GND→GND SDA→A4 SCL→A5
//       TM1637: VCC→5V GND→GND CLK→10 DIO→9
#include <Wire.h>       // 引入 I2C 通信库（DS1307 时钟模块必需）
#include <RTClib.h>     // 引入 DS1307 实时时钟库
#include <DHT.h>        // 引入 DHT11 温湿度传感器库
#include <TM1637.h>     // 引入师兄指定的 Grove TM1637 四位数码管库

#define CLK 10           // TM1637 CLK 接 10
#define DIO 9            // TM1637 DIO 接 9
#define GROUP_NUMBER 3   // 本组组号

TM1637 tm1637(CLK, DIO);
RTC_DS1307 RTC;

void setup() {
  Serial.begin(9600);
  if (!RTC.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  tm1637.init();
  tm1637.set(BRIGHTEST);   // BRIGHTEST = 7, TYPICAL = 2, DARKEST = 0
  tm1637.point(POINT_ON);  // 中间冒号常亮
}

void loop() {
  DateTime theTime = RTC.now();
  int hour   = theTime.hour();    // 老师原代码漏了 hour 赋值，这里补上
  int minute = theTime.minute();
  int second = theTime.second();

  // 修改1：数码管显示"组号:分钟"
  // 前两位：组号（不足两位补 0），后两位：分钟
  tm1637.display(0, GROUP_NUMBER / 10);
  tm1637.display(1, GROUP_NUMBER % 10);
  tm1637.display(2, minute / 10);
  tm1637.display(3, minute % 10);

  // 修改2：串口先输出组号，再输出时间数据
  Serial.print("组号: ");
  Serial.print(GROUP_NUMBER);
  Serial.print("  时间: ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);

  delay(1000);   // 1 秒刷新一次
}
