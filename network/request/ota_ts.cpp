#include "ota_ts.hpp"
#include "utils/log.hpp"

#include <ArduinoHttpClient.h>
#include <SSLClientESP32.h>
#include <ArduinoJson.h>

#define PINICORE_TAG_OTA    "pcore_ota_ts"

#define OTA_TS_HOST "ota.trigger.systems"
#define OTA_TS_PORT 443

#define OTA_TS_USER ""
#define OTA_TS_PASS ""

#define OTA_TS_CHECKUPDATE_BUFFER_MAX_SIZE  128
#define OTA_TS_CHECKUPDATE_URI_MAX_SIZE     128

#define URI "/ota/check/?machineKey=%s&version=%d"


void OTATS::addCertificate(const char* certificate) {
    m_certificate = certificate;
}

bool OTATS::checkUpdate() {
    SSLClientESP32 ssl_client(getClient());
    ssl_client.setCACert(m_certificate);

    char uri[OTA_TS_CHECKUPDATE_URI_MAX_SIZE];
    if (snprintf(uri, sizeof(uri), URI, getUniqueID(), getCurrentFirmware()) >= sizeof(uri)) {
        LOG_E(PINICORE_TAG_OTA, "URI buffer too small, aborting check update");
        return false;
    }

    // Request
    HttpClient http(ssl_client, OTA_TS_HOST);
    http.beginRequest();
    http.get(uri);
    http.sendBasicAuth(OTA_TS_USER, OTA_TS_PASS);
    http.endRequest();

    // Response
    int status = http.responseStatusCode();
    http.skipResponseHeaders();

    char body[OTA_TS_CHECKUPDATE_BUFFER_MAX_SIZE];
    size_t bodySize = http.read((uint8_t*)body, OTA_TS_CHECKUPDATE_BUFFER_MAX_SIZE);
    body[bodySize] = 0; // ensure null termination

    LOG_D(PINICORE_TAG_OTA, "Received: [status: %d] [body (%d): %s]", status, bodySize, body);
    if (status == 404) {
        return false;
    }
    
    // JSON parsing
    JsonDocument json;
    DeserializationError jsonError = deserializeJson(json, body);
    if (jsonError) {
        const char* errorMsg = jsonError.c_str();
        LOG_D(PINICORE_TAG_OTA, "JSON deserialization error: '%s'", errorMsg);
        return false;
    }
    
    p_versionAvailable = -1; // Invalidate previous update version check

    // Get update information
    if (json["success"].as<bool>()) {
        // available update version
        p_versionAvailable = json["firmware"]["version"];

        // available update URI path
        strncpy(m_uriAvailableUpdate, json["firmware"]["url"], OTA_TS_AVAILABLEUPDATE_URI_MAX_SIZE);

        LOG_I(PINICORE_TAG_OTA, "Found update: [version: %d] [uri: '%s']", p_versionAvailable, m_uriAvailableUpdate);
        return true;
    }
    return false;
}

EOTAUpdateStatus OTATS::update() {
    if (p_versionAvailable == -1) {
        return EOTAUpdateStatus::OTA_UPDATE_NOT_AVAILABLE;
    }

    SSLClientESP32 ssl_client(getClient());
    ssl_client.setCACert(m_certificate);

    // Request
    HttpClient http(ssl_client, OTA_TS_HOST);
    http.connectionKeepAlive();
    http.beginRequest();
    http.get(m_uriAvailableUpdate);
    http.sendBasicAuth(OTA_TS_USER, OTA_TS_PASS);
    http.endRequest();

    // Response
    int status = http.responseStatusCode();
    char headers[256];
    int headersIdx = 0;
    while (http.headerAvailable()) {
        LOG_T(PINICORE_TAG_OTA, "%s: %s", http.readHeaderName(), http.readHeaderValue());
        /*
        headers[headersIdx++] = http.readHeader();
        */
        if (http.endOfHeadersReached())
            break;
    }
    headers[headersIdx] = 0;

    LOG_D(PINICORE_TAG_OTA, "Received: [status: %d] [headers (%d): %s]", status, headersIdx-1, headers);

    return EOTAUpdateStatus::OTA_FAILED;
}
