/*
 * OTA.h
 *
 *  Created on: Aug. 9, 2021
 *      Author: jareas
 */

#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPClient.h>

#ifdef DEBUGGER
#include "Debugger.h"
#endif

#ifndef SRC_FSEOTA_H_
#define SRC_FSEOTA_H_

class FSEOTA {
public:
	FSEOTA();
	virtual ~FSEOTA();
	void begin(String hostname);
	void handle();
	void onEnd();
	void onProgress(unsigned int progress, unsigned int total);
	void onStart();
	void onError(ota_error_t error);
	void otaUpdateWeb(String host, int port, String file, String fingerprint);
protected:
	ESP8266WiFiMulti _WiFiMulti;
#ifdef DEBUGGER
	void setDebugger(Debugger debug);
#endif
private:
#ifdef DEBUGGER
	Debugger _debug;
#endif

};

#endif /* SRC_FSEOTA_H_ */
