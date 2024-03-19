# Smart-Aquarium
Optimizing Aquarium Environment: IoT-Enabled Temperature Monitoring and PID-Controlled Water Cooling System

there is 2 code, one for arduino and other for esp8266, and they are communicate with softwareserial library, collected 2 DS18B20 sensor data with onewire library and then calculate PID power for chiler to make aquarium water temperature stable with the given setpoin, and then you can monitor the aquarium from blynk.

you can change the PID setpoint by change the value in line 14.
