#include "FSEOTA.h"

FSEOTA ota;


/*
 * define values for updating from a wehost
 */

#define OTA_FILE "/myrepo/firmware.bin"
#define OTA_HOST "raw.githubusercontent.com"

#define OTA_PORT 443
#define OTA_FINGERPRINT "02 AC 5C 26 6A 0B 40 9B 8F 0B 79 F2 AE 46 25 77" // this is git raw fingerprint
#define HOSTNAME "mydevicehost" //this will translate to mydevicehost.local in your platformio.ini file

/* Platformio entries
upload_protocol = espota
upload_port = mydevicehost.local
*/

void setup(){
	ota.begin(HOSTNAME);
	// update the device from the internet
}

void loop() {
	ota.handle();
}
