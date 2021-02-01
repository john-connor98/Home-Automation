/* 
Controlling light fan and switch from mobile blynk application and sending notification
when the state of the appliances changes. 

*/

    #define BLYNK_PRINT DebugSerial
    #include <SoftwareSerial.h>
    #include <BlynkSimpleStream.h>
    SoftwareSerial DebugSerial(2, 3); // RX, TX

    char auth[] = "5w_TAruJhflZsh5DaismpWpLq0f873nh";

    int light_prev = 0, light_curr = 0;   /// control light
    int fan_prev = 0, fan_curr = 0;       /// control fan
    int switch_prev = 0, switch_curr = 0; /// control switch
    int pir_state = 0; bool lock = true;  /// pir sensor
    int temp_reading = 0, critical_temp = 45;  /// temperature sensor temppin

    int pir_pin = 8;
    int fan_pin = 12;
    int light_pin = 13;
    int temp_pin = 0;
    int switch_pin = 11;

void setup() {
    DebugSerial.begin( 9600 );
    Serial.begin( 9600 );
    
    Blynk.begin( Serial, auth );
    analogReference( INTERNAL );
    
    pinMode( pir_pin, INPUT );
    pinMode( fan_pin, INPUT );
    pinMode( light_pin, INPUT );
    pinMode( switch_pin, INPUT );
    delay(50);
}

void loop() {
    Blynk.run();

    fan_curr = digitalRead( fan_pin );
    pir_state = digitalRead( pir_pin );
    light_curr = digitalRead( light_pin );           
    switch_curr = digitalRead( switch_pin );
    temp_reading = analogRead( temp_pin );

    // light control
    if(light_curr != light_prev) {
        if(light_curr == 1 && light_prev == 0) {
            Blynk.notify("Turning ON the Light");
        } else {
            Blynk.notify("Turning OFF the Light");
        }
        light_prev = light_curr;
    }
    
    // fan control
    if(fan_curr != fan_prev) {
        if(fan_curr == 1 && fan_prev == 0) {
            Blynk.notify("Turning ON the Fan");
        } else {
            Blynk.notify("Turning OFF the Fan");
        }
        fan_prev = fan_curr;
    }
    
    // switch control
    if(switch_curr != switch_prev) {
        if(switch_curr == 1 && switch_prev == 0) {
            Blynk.notify("Turning ON the Switch");
        } else {
            Blynk.notify("Turning OFF the Switch");
        }
        switch_prev = switch_curr;
    }
    
    // pir conrol
    if(pir_state == 1) {
        if(lock) {
            Blynk.notify("..Motion Detected..");
            lock = false;
        }
    } else if(pir_state == 0) {
        lock = true;
    }
 
    // temperature control
    temp_reading = temp_reading * 1.07;   // reference voltage=1.1 so 1100/1024
    temp_reading = temp_reading / 10;    // each 10mV equals 1 degree celsius

    if(temp_reading > critical_temp) {
        Blynk.notify("..Temperature alert..");
    }
}
