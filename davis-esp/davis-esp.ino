#include "esp_wifi.h"
#include "variables.h"
#include "aws_request.h"
#include "davis.h"
#include <NTPClient.h>

using namespace aws_request;

char body[400] = {0};
AWSRequest request(AWSRequestParameters{
    .method = "POST",
    .body = body,
    .host = "sns.ap-southeast-2.amazonaws.com",
    .path = "/",
    .region = "ap-southeast-2",
    .service = "sns",
    .access_key_id = AWS_ACCESS_KEY_ID,
    .secret_access_key = AWS_SECRET_ACCESS_KEY});
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 7200000);

void setup()
{
  Serial.begin(115200);
  davis_setup();
  delay(500);
#ifdef ESP8266
  timeClient.begin();
#endif
}

void loop()
{
  if (startWiFi(WIFI_SSID, WIFI_PASSWORD))
  {
    timeClient.begin();
    timeClient.update();
    time_t timestamp = timeClient.getEpochTime();
    sprintf(body, "Version=2010-03-31&Action=Publish&TopicArn=arn:aws:sns:ap-southeast-2:605337347611:davis-weather-ingest&Message=01%08x0000", timestamp);
    size_t prefix_len = strlen(body);
    if (davis_go(body + prefix_len)) {
      Serial.printf("[General] Sending SNS message %s at timestamp %d.\n", body, timestamp);
      AWSResponse response = request.send(timestamp);
      Serial.printf("[General] Result: %d\n", response.status);
    } else {
      Serial.printf("[General] Failed to get data from Davis.\n");
    }

    stopWiFi();
  }
  Serial.printf("[General] Waiting for 2 minutes...\n");
  delay(120000);
}
