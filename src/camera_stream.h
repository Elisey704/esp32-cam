#include "esp_camera.h"
// Конфигурация пинов для AI-Thinker ESP32-CAM
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

void cameraInit(){
      // Инициализация камеры
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // Разрешение для инициализации
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 5;
  config.fb_count = 2;

  // Инициализация камеры
  esp_err_t err = esp_camera_init(&config);
  while (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x", err);
    return;
  }

}

void webStream(WiFiClient client){
    // Заголовки HTTP-ответа
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
    client.print(response);

    while (client.connected()) {
      // Получение кадра с камеры
      camera_fb_t *fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        //continue;
      }

      // Отправка кадра
      client.print("--frame\r\n");
      client.print("Content-Type: image/jpeg\r\n\r\n");
      client.print("");
      client.write(fb->buf, fb->len);
      client.print("\r\n");
      
      // Освобождение буфера
      esp_camera_fb_return(fb);

      // Задержка для стабильности потока
      //delay(16);
    }
    
    //client.stop();
    Serial.println("Client disconnected");
}
