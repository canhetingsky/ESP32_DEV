// #include <Arduino.h>
// #include <WebServer.h>
#include <WiFiManager.h>

// 定义MQTT服务器地址、端口号和主题
char mqtt_server[40] = "192.168.0.1";
char mqtt_port[6] = "1883";
char mqtt_topic[40] = "topic";

// 建立WiFiManager对象
WiFiManager wifiManager;
// 添加需要配置的参数
WiFiManagerParameter custom_mqtt_server("server", "MQTT server", mqtt_server, 40);
WiFiManagerParameter custom_mqtt_port("port", "MQTT port", mqtt_port, 6);
WiFiManagerParameter custom_mqtt_topic("topic", "MQTT topic", mqtt_topic, 40);

WiFiManagerParameter custom_html("<p style=\"color:pink;font-weight:Bold;\">This Is Custom HTML</p>");

const char *bufferStr = R"(
  <!-- INPUT CHOICE -->
  <br/>
  <p>Select Choice</p>
  <input style='display: inline-block;' type='radio' id='choice1' name='program_selection' value='1'>
  <label for='choice1'>Choice1</label><br/>
  <input style='display: inline-block;' type='radio' id='choice2' name='program_selection' value='2'>
  <label for='choice2'>Choice2</label><br/>

  <!-- INPUT SELECT -->
  <br/>
  <label for='input_select'>Label for Input Select</label>
  <select name="input_select" id="input_select" class="button">
  <option value="0">Option 1</option>
  <option value="1" selected>Option 2</option>
  <option value="2">Option 3</option>
  <option value="3">Option 4</option>
  </select>
  )";

WiFiManagerParameter custom_html_inputs(bufferStr);

void saveParamCallback()
{
    Serial.println("[CALLBACK] saveParamCallback fired");
    if (wifiManager.server->hasArg("server"))
    {
        sprintf(mqtt_server, "%s", wifiManager.server->arg("server"));
        // mqtt_server = custom_mqtt_server.getValue();
        Serial.println("mqtt_server: ");
        Serial.println(mqtt_server);
    }
    if (wifiManager.server->hasArg("port"))
    {
        sprintf(mqtt_port, "%s", wifiManager.server->arg("port"));
        Serial.println("mqtt_port: ");
        Serial.println(mqtt_port);
    }
    if (wifiManager.server->hasArg("topic"))
    {
        sprintf(mqtt_topic, "%s", wifiManager.server->arg("topic"));
        Serial.println("mqtt_topic: ");
        Serial.println(mqtt_topic);
    }
    if (wifiManager.server->hasArg("input_select"))
    {
        sprintf(mqtt_topic, "%s", wifiManager.server->arg("input_select"));
        Serial.println("input_select: ");
        Serial.println(mqtt_topic);
    }
}

// void setWebServerCallback()
// {
//     Serial.println("[CALLBACK] setWebServerCallback fired");
//     Serial.print("IP address:\t");
//     Serial.println(WiFi.localIP()); // IP
// }

void handleRoute()
{
    Serial.println("[HTTP] handle route");
    wifiManager.server->send(200, "text/plain", "hello from user code");
}

void setWebServerCallback()
{
    wifiManager.server->on("/custom", handleRoute); // this is now crashing esp32 for some reason
    wifiManager.server->on("/info", handleRoute);   // you can override wm!
}

void WiFiManagerParameter_init(void)
{

    // 初始化wifi管理器并将参数添加到它上面
    wifiManager.addParameter(&custom_mqtt_server);
    wifiManager.addParameter(&custom_mqtt_port);
    wifiManager.addParameter(&custom_mqtt_topic);

    wifiManager.addParameter(&custom_html);
    wifiManager.addParameter(&custom_html_inputs);
}

void setup()
{
    Serial.begin(115200);

    WiFiManagerParameter_init();
    wifiManager.setSaveParamsCallback(saveParamCallback);
    wifiManager.setWebServerCallback(setWebServerCallback);

    Serial.println("start to connect to wifi...");
    wifiManager.resetSettings(); // 重置wifi设置
    // 自动连接WiFi。以下语句的参数是连接ESP8266时的WiFi名称
    wifiManager.autoConnect("AutoConnectAP");

    // 如果您希望该WiFi添加密码，可以使用以下语句：
    // wifiManager.autoConnect("AutoConnectAP", "12345678");
    // 以上语句中的12345678是连接AutoConnectAP的密码

    // WiFi连接成功后将通过串口监视器输出连接成功信息
    Serial.println("");
    Serial.print("ESP8266 Connected to ");
    Serial.println(WiFi.SSID()); // WiFi名称
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP()); // IP
    delay(2000);
    // Serial.println("loop app");
    wifiManager.startWebPortal(); // 开启webserver，用于配置参数
    Serial.println("WiFi server config");
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP()); // IP
}

void loop()
{
    static uint32_t last_ms = millis();
    if (millis() - last_ms > 1000)
    {
        last_ms = millis();
        Serial.println("loop");
    }
    wifiManager.process();
}
