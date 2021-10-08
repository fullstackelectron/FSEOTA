/*
 * OTA.cpp
 *
 *  Created on: Aug. 9, 2021
 *      Author: jareas
 */

#include <FSEOTA.h>
#include <ESP8266mDNS.h>

FSEOTA::FSEOTA() {
	// TODO Auto-generated constructor stub

}

FSEOTA::~FSEOTA() {
	// TODO Auto-generated destructor stub
}
#ifdef DEBUGGER
void FSEOTA::setDebugger(Debugger debug) {
	_debug = debug;
}
#endif

void FSEOTA::begin(String hostname) {
	WiFi.hostname(hostname);
	if (!MDNS.begin(hostname)) {             // Start the mDNS responder for HOSTNAME.local
		Serial.println("Error setting up MDNS responder!");
	} else {
		Serial.println("MDNS responder is UP");
		Serial.println(hostname);
		Serial.println(WiFi.localIP());
	}
	MDNS.addService("esp", "tcp", 8266);

	ArduinoOTA.setHostname(hostname.c_str());
	ArduinoOTA.onStart(std::bind(&FSEOTA::onStart, this));
//	ArduinoOTA.onStart([]() {
//		// NOTE: if updating FS this would be the place to unmount FS using FS.end()
//		Serial.println("Start updating ");
//	});
//	ArduinoOTA.onEnd([]() {
//		Serial.println("\nEnd");
//	});
	ArduinoOTA.onProgress(std::bind(&FSEOTA::onProgress, this, std::placeholders::_1, std::placeholders::_2));
	ArduinoOTA.onEnd(std::bind(&FSEOTA::onEnd, this));
//	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
//		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
//	});
	ArduinoOTA.onError(std::bind(&FSEOTA::onError, this, std::placeholders::_1));
	ArduinoOTA.begin();

//	ESPhttpUpdate.onError(std::bind(&FSEOTA::onError, this, std::placeholders::_1));
	_WiFiMulti.addAP("MYDEVICE","myesp");
}

void FSEOTA::onError(ota_error_t error) {
	Serial.printf("Error[%u]: ", error);
	if (error == OTA_AUTH_ERROR) {
	  Serial.println("Auth Failed");
	} else if (error == OTA_BEGIN_ERROR) {
	  Serial.println("Begin Failed");
	} else if (error == OTA_CONNECT_ERROR) {
	  Serial.println("Connect Failed");
	} else if (error == OTA_RECEIVE_ERROR) {
	  Serial.println("Receive Failed");
	} else if (error == OTA_END_ERROR) {
	  Serial.println("End Failed");
	}
}

void FSEOTA::onEnd() {
	Serial.println("\nEnd");
}

void FSEOTA::onStart() {
	Serial.println("Start updating ");
}

void FSEOTA::onProgress(unsigned int progress, unsigned int total) {
	Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
}

void FSEOTA::handle() {
	ArduinoOTA.handle();
}

void FSEOTA::otaUpdateWeb(String host, int port, String file, String fingerprint) {
	Serial.printf("Preparing update from: %s", file.c_str());
	Serial.println("");
	if ((_WiFiMulti.run() == WL_CONNECTED)) {
		// configure time
		configTime(4 * 3600, 0, "pool.ntp.org");
		WiFiClientSecure client;
		client.setInsecure();
		Serial.print("connecting to ");
		Serial.println(host);
		Serial.print("On port ");
		Serial.println(port);
		if (!client.connect(host, port)) {
			Serial.println("connection failed");
			return;
		}

		if (client.verify(fingerprint.c_str(), host.c_str())) {
			Serial.println("certificate matches");
		} else {
			Serial.println("certificate doesn't match");
		return;
		}

		ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
		ESPhttpUpdate.onStart(std::bind(&FSEOTA::onStart, this));
		ESPhttpUpdate.onEnd(std::bind(&FSEOTA::onEnd, this));
		ESPhttpUpdate.onProgress(std::bind(&FSEOTA::onProgress, this, std::placeholders::_1, std::placeholders::_2));
		String url = "https://" + host + file;
		t_httpUpdate_return ret = ESPhttpUpdate.update(client, url);
		switch (ret) {
		  case HTTP_UPDATE_FAILED:
			Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
			break;

		  case HTTP_UPDATE_NO_UPDATES:
			Serial.println("HTTP_UPDATE_NO_UPDATES");
			break;

		  case HTTP_UPDATE_OK:
			Serial.println("HTTP_UPDATE_OK");
			break;
		}
	}
}
