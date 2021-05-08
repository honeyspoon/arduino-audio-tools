/**
 * @file url_raw-I2S_external_dac.ino
 * @author Phil Schatzmann
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/url_raw-I2S_externel_dac/README.md 
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

#include "WiFi.h"
#include "AudioTools.h"

using namespace audio_tools;  

UrlStream music;          // Music Stream
I2S<int16_t> i2s;         // I2S 
I2SStream i2s_stream(i2s);// I2S as Stream
StreamCopy copier(i2s_stream, music, 1024); // copy music to i2s_stream


// Arduino Setup
void setup(void) {
  Serial.begin(115200);

  // connect to WIFI
  WiFi.begin("network", "pwd");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500); 
  }
 
  // open music stream
  music.begin("https://pschatzmann.github.io/arduino-audio-tools/resources/audio-8000.raw");

  // start I2S with external DAC
  Serial.println("\nstarting I2S...");
  I2SConfig cfg = i2s.defaultConfig(TX_MODE);
  cfg.sample_rate = 8000;
  i2s.begin(cfg);
}

// Arduino loop - repeated processing: copy input stream to output stream
void loop() {
  int len = copier.copy();
  if (len){
      Serial.print(".");
  } else {
      i2s.end();
      Serial.println("\nCopy ended");
      delay(10000);
  }
}