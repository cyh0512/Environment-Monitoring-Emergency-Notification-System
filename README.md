# Environment-Monitoring-Emergency-Notification-System

## Monitor risk factors in near house environment
- Add in temperature and humidity, UVI, AQI, earthquake detection, smoke detection, display on the screen and distinguish the hazard level in different colors, so that the elderly can realize at a glance.

## Notify family and friends when a emergency occurs
- When the sensor value exceeds the threshold, immediately send a warning message to the family or friends by call, SMS, and LINE.
- Emergency alarm will continue until the elder actively push down the safety button to report his/her safety.

This system combines fast dialing, home safety, environmental monitoring and emergency notification. 
It is suitable to install in home for the family whose elder needs care.

## Method
- The mobile app obtains the open data of UVI and AQI from the Environmental Protection Agency(EPA), and combine with the environmental sensors value in the system.
- After receiving the data, the value exceeding threshold is automatically written into the Google Form URL for response. 
- By sending HTTP GET request, we can transmit the emergency message and data to the Google Form for recording.
- Google Apps Scripts automatically receives Google Form responses and sends messages to specific friends or groups via LINE Notify connection service.
