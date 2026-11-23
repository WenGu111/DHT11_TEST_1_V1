// ============================================================
// 实验1：DHT11 温湿度传感器（修改版）
// 功能：
//  1. 串口输出传感器数据前，先输出本组组号
//  2. 温度升高 LED 闪烁变快，温度降低闪烁变慢
// 接线：
//  DHT11: VCC→5V  GND→GND  S→数字引脚2
//         （若为 4 脚裸传感器，S 与 VCC 之间接 10kΩ 上拉电阻）
//  LED  : 使用 UNO 板载 13 脚
// 库  ：DHT sensor library
// ============================================================
#include "DHT.h"

#define DHTPIN 2        // DHT11 数据脚 S 接数字引脚 2
#define DHTTYPE DHT11   // 传感器类型
#define LED_PIN 13      // 温度指示灯：UNO 板载 LED
#define GROUP_NUMBER 3  // 本组组号

DHT dht(DHTPIN, DHTTYPE);

unsigned long lastSample = 0;   // 上次采样的时间
unsigned long lastBlink  = 0;   // 上次翻转 LED 的时间
int blinkInterval = 500;        // 闪烁间隔（毫秒），越小闪得越快
bool ledState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
  Serial.print("组号: ");
  Serial.println(GROUP_NUMBER);
  Serial.println("DHT11 实验开始");
}

void loop() {
  unsigned long now = millis();

  // ---- 每 2 秒采样一次（DHT11 采样间隔不能小于 1 秒） ----
  if (now - lastSample >= 2000) {
    lastSample = now;

    float t = dht.readTemperature();   // 温度 ℃
    float h = dht.readHumidity();      // 湿度 %

    if (isnan(t) || isnan(h)) {        // 读取失败
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // 修改1：先输出组号，再输出传感器数据
    Serial.print("组号: ");
    Serial.print(GROUP_NUMBER);
    Serial.print("  温度: ");
    Serial.print(t);
    Serial.print(" C   湿度: ");
    Serial.print(h);
    Serial.println(" %");

    // 修改2：温度 -> 闪烁间隔
    // 15℃ 约 1 秒闪一次，35℃ 约 0.1 秒闪一次
    // 温度上升 -> 间隔变小 -> 闪烁变快；温度下降 -> 闪烁变慢
    blinkInterval = map(t, 15, 35, 1000, 100);
    blinkInterval = constrain(blinkInterval, 100, 1000);
  }

  // ---- 按当前温度决定的节奏闪烁（非阻塞，不影响采样） ----
  if (now - lastBlink >= blinkInterval) {
    lastBlink = now;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}
