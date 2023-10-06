/*************************************************************
  This is an example of how to use the ESP32 WiFiManager library 
  with the Blynk library. With this code, your ESP32 will be able 
  to automatically connect to a known network or set up an access 
  point for setting up WiFi credentials and then communicate to 
  the Blynk server.

  WiFiManager library: https://github.com/tzapu/WiFiManager
  Blynk library: https://github.com/blynkkk/blynk-library


  https://github.com/AbooodyN/esp-arduino
 *************************************************************/

/* Fill-in information from Blynk Device Info here,
   Your Device Information MUST be at the TOP of the code */
#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME         "YourTemplateName"
#define BLYNK_AUTH_TOKEN            "YourAuthToken"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32_SSL.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

//WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
WiFiManager wifiManager;

// The Access Point name and password.
const char* wifiManagerAPName = "ESP32-AP";
const char* wifiManagerAPPass = "password";

#define ESP_Boot_PIN 0   // Button uesed for reseting the WiFi settings.
  // (D9 on Xiao 32C3) , (GPIO 0 on ESP32,ESP32S.)

//WiFiClient espClient;

//String string_ssid = ""; // string to store wifi name after connecting to WiFi
//String string_pass = ""; // string to store wifi password after connecting to WiFi


unsigned long currentMillis  = millis();
unsigned long previousMillis = 0;


void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200); 
    Serial.println(F("\nBooting up..."));
    
    pinMode(ESP_Boot_PIN, INPUT_PULLUP);


    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

//    std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
//    std::vector<const char *> menu = {"wifi","info","sep","exit"};
//    wifiManager.setMenu(menu);
    wifiManager.setClass("invert"); // dark theme
    //wifiManager.setDebugOutput(false);
    wifiManager.setConnectTimeout(60);  //Timeout to try connecting to Wifi in seconds, then reopen access point.

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
//     wifiManager.resetSettings();

    if(!wifiManager.autoConnect(wifiManagerAPName, wifiManagerAPPass)) {
        Serial.println(F("Failed to connect to WiFi. Restarting..."));
        delay(2000);
        ESP.restart();  // restart the ESP32
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println(F("...connected to WiFi... :)"));
    }
//  string_ssid = WiFi.SSID();  // get the SSID to the connected WiFi.
//  string_pass = WiFi.psk();   // get the password to the connected WiFi.

  // copy the sting to const char*, to use for Blynk.begin() function.
  const char*  ssid = WiFi.SSID().c_str();
  const char*  pass = WiFi.psk().c_str();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 443);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8443);

}

void loop() {

  Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!

  //check if Boot button is preased; 
  //If yes, reset settings an restart ESP to reconfigure WiFi. 
  if (digitalRead(ESP_Boot_PIN) == LOW) {
    Serial.print(F("\nWi-Fi reset Button pressed, \nOpening Access Point named: "));
    Serial.println(wifiManagerAPName);
    WiFi.disconnect();
    delay(1000);
    //to reset settings, uncomment the line bellow,"wifiManager.resetSettings();". 
    wifiManager.resetSettings();
    delay(3000);
    ESP.restart();
  }
//==========================================================
    // put your main code here, to run repeatedly:




}
