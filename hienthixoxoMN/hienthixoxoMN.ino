#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

const char* ssid = "nhau thoi ae";
const char* password = "nayhetmangroi";

String urls[] = {
    "https://xskt.com.vn/xstg",         // Chủ nhật
    "https://xskt.com.vn/xshcm-xstp",   // Thứ 2
    "https://xskt.com.vn/xsvt",         // Thứ 3
    "https://xskt.com.vn/xsdn",         // Thứ 4
    "https://xskt.com.vn/xstn",         // Thứ 5
    "https://xskt.com.vn/xsbd",         // Thứ 6
    "https://xskt.com.vn/xshcm-xstp"    // Thứ 7
};

String extractString(String source, String delimiter, int offset, int length) {
    int startIndex = source.indexOf(delimiter) + offset;
    if (startIndex >= 0 && startIndex + length <= source.length()) {
        return source.substring(startIndex, startIndex + length);
    }
    return "";
}

void fetchData3(String url) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
        String html = http.getString();
        Serial.println(html);

        String prize8 = extractString(html, "G8", 15, 2);
        String prize7 = extractString(html, "G7", 14, 3);
        String prize6 = extractString(html, "G6", 14, 14);
        String prize5 = extractString(html, "G5", 14, 4);
        String prize4 = extractString(html, "G4", 26, 17);
        String prize4_1 = extractString(html, "G4", 47, 23);
        String prize3 = extractString(html, "G3", 14, 11);
        String prize2 = extractString(html, "G2", 14, 5);
        String prize1 = extractString(html, "G1</td><td><p>", 14, 5);
        String prizeDB = extractString(html, "ĐB", 21, 6);

        Serial.println("Kết quả xổ số:");
        Serial.println(prize8);
        Serial.println(prize7);
        Serial.println(prize6);
        Serial.println(prize5);
        Serial.println(prize4);
        Serial.println(prize4_1);
        Serial.println(prize3);
        Serial.println(prize2);
        Serial.println(prize1);
        Serial.println(prizeDB);

        Serial1.println("G8:" + prize8);
        Serial1.println("G7:" + prize7);
        Serial1.println("G6:" + prize6);
        Serial1.println("G5:" + prize5);
        Serial1.println("G4:" + prize4);
        Serial1.println("G4: " + prize4_1);
        Serial1.println("G3:" + prize3);
        Serial1.println("G2:" + prize2);
        Serial1.println("G1:" + prize1);
        Serial1.println("DB:" + prizeDB);
    } else {
        Serial.println("Failed to connect to server. HTTP code: " + String(httpCode));
    }
    
    http.end();
}

void setup() {
    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8N1, 17, 16);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    delay(5000);

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }

    int dayOfWeek = timeinfo.tm_wday;
    int hour = timeinfo.tm_hour;
    int minute = timeinfo.tm_min;

    Serial.print("Today's date: ");
    Serial.print(timeinfo.tm_mday);
    Serial.print("/");
    Serial.print(timeinfo.tm_mon + 1);
    Serial.print("/");
    Serial.println(timeinfo.tm_year + 1900);

    if (hour < 16 || (hour == 16 && minute < 30)) {
        dayOfWeek = (dayOfWeek - 1 + 7) % 7;  // Lùi lại một ngày
        if (timeinfo.tm_mday == 1) {
            timeinfo.tm_mon -= 1;
            if (timeinfo.tm_mon < 0) {
                timeinfo.tm_mon = 11;
                timeinfo.tm_year -= 1;
            }
            timeinfo.tm_mday = 30;  // Số ngày trong tháng cần được điều chỉnh phù hợp
        } else {
            timeinfo.tm_mday -= 1;
        }
    }

    String url = urls[dayOfWeek];
    String formattedDate = "/ngay-" + String(timeinfo.tm_mday) + "-" + String(timeinfo.tm_mon + 1) + "-" + String(timeinfo.tm_year + 1900);
    Serial.println("Fetching data from URL: " + url + formattedDate);
    fetchData3(url + formattedDate);
}

void loop() {
    // Không làm gì trong vòng lặp
}
