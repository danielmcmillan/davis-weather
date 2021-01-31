#include "esp_wifi.h"
#include "variables.h"
#include "aws_request.h"
#include "davis.h"

using namespace aws_request;

char body[350] = {0};
AWSRequest request(AWSRequestParameters{
    .method = "POST",
    .body = body,
    .host = "sns.ap-southeast-2.amazonaws.com",
    .path = "/",
    .region = "ap-southeast-2",
    .service = "sns",
    .access_key_id = AWS_ACCESS_KEY_ID,
    .secret_access_key = AWS_SECRET_ACCESS_KEY});
unsigned int hello_count = 1;

void setup()
{
  Serial.begin(115200);
  davis_setup();
  delay(500);
}

void loop()
{
  if (startWiFi(WIFI_SSID, WIFI_PASSWORD))
  {
    sprintf(body, "Version=2010-03-31&Action=Publish&TopicArn=arn:aws:sns:ap-southeast-2:605337347611:davis-weather-ingest&Message=Hello number %u:", hello_count);
    if (davis_go(body + strlen(body))) {
      Serial.printf("[General] Sending SNS message %s.\n", body);
      hello_count += 1;
      AWSResponse response = request.send();
      Serial.printf("[General] Result: %d\n", response.status);
    } else {
      Serial.printf("[General] Failed to get data from Davis.\n");
    }

    stopWiFi();
  }
  Serial.printf("[General] Waiting for 2 minutes...\n");
  delay(120000);
}
