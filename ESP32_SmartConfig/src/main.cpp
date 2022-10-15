#include <WiFi.h>

#define MAX_RETRY 10

void smart_config(void)
{
    // Init WiFi as Station, start SmartConfig
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();

    // Wait for SmartConfig packet from mobile
    Serial.println("Waiting for SmartConfig.");
    while (!WiFi.smartConfigDone())
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("SmartConfig received.");

    // Wait for WiFi to connect to AP
    Serial.println("Waiting for WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    WiFi.setAutoConnect(true); // 设置自动连接
}

bool connect_wifi(void)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(); //启动WIFI连接
    Serial.println("Connection WIFI");

    int retry_count = 0;
    while (retry_count < MAX_RETRY)
    {
        delay(500);
        Serial.print(".");
        retry_count++;
        if (WiFi.status() == WL_CONNECTED) //检查连接状态
        {
            return true;
        }
    }
    return false;
}

void setup_wifi(void)
{
    if (!connect_wifi())
    {
        smart_config();
    }

    Serial.println("");
    Serial.print("WiFi connected: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    setup_wifi();
}

void loop()
{
    // put your main code here, to run repeatedly:
    Serial.println("hello world!");
    delay(1000);
}
