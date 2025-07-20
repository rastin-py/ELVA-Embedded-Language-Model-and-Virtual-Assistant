#include "driver/i2s_std.h" // mic
#include <SD.h>            // sd
#include <SPI.h>
#include "Audio.h"         // speaker
#include "driver/gpio.h"
#include "FS.h"
#include <WiFi.h>
#include "esp_heap_caps.h"

// sd
#define SD_CS GPIO_NUM_5

// sound recorder
#define I2S_WS_PIN  GPIO_NUM_15  // LRCL
#define I2S_BCK_PIN GPIO_NUM_14  // BCLK
#define I2S_DATA_PIN GPIO_NUM_33 // DOUT
i2s_chan_handle_t rx_handle;

// sound player
#define I2S_DOUT GPIO_NUM_25
#define I2S_BCLK GPIO_NUM_27
#define I2S_LRC  GPIO_NUM_26

const char* ssid = "Rastin";
const char* password = "12348765";

WiFiServer server(12345);

const char* inputFileName = "/input.wav";
const char* outputFileName = "/output.mp3";

void writeWavHeader(File file, uint32_t sampleRate, uint32_t dataSize) {
    uint8_t header[44] = {
        'R', 'I', 'F', 'F',                  // ChunkID
        0, 0, 0, 0,                          // ChunkSize (placeholder)
        'W', 'A', 'V', 'E',                  // Format
        'f', 'm', 't', ' ',                  // Subchunk1ID
        16, 0, 0, 0,                         // Subchunk1Size (PCM header size)
        1, 0,                                // AudioFormat (1 for PCM)
        1, 0,                                // NumChannels (1 channel for mono)
        (uint8_t)(sampleRate & 0xFF),
        (uint8_t)((sampleRate >> 8) & 0xFF),
        (uint8_t)((sampleRate >> 16) & 0xFF),
        (uint8_t)((sampleRate >> 24) & 0xFF),
        (uint8_t)((sampleRate * 2) & 0xFF),
        (uint8_t)(((sampleRate * 2) >> 8) & 0xFF),
        (uint8_t)(((sampleRate * 2) >> 16) & 0xFF),
        (uint8_t)(((sampleRate * 2) >> 24) & 0xFF),
        2, 0,                                // BlockAlign
        16, 0,                               // BitsPerSample
        'd', 'a', 't', 'a',                  // Subchunk2ID
        (uint8_t)(dataSize & 0xFF),
        (uint8_t)((dataSize >> 8) & 0xFF),
        (uint8_t)((dataSize >> 16) & 0xFF),
        (uint8_t)((dataSize >> 24) & 0xFF),
    };

    uint32_t totalFileSize = dataSize + 36;
    header[4] = (uint8_t)(totalFileSize & 0xFF);
    header[5] = (uint8_t)((totalFileSize >> 8) & 0xFF);
    header[6] = (uint8_t)((totalFileSize >> 16) & 0xFF);
    header[7] = (uint8_t)((totalFileSize >> 24) & 0xFF);

    file.write(header, 44);
}

File setupI2SMic() {
    File inputFile = SD.open(inputFileName, FILE_WRITE);
    while (!inputFile) {
        Serial.println("Failed to open WAV file!");
        inputFile = SD.open(inputFileName, FILE_WRITE);
        delay(100);
    }

    writeWavHeader(inputFile, 44100, 0);

    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, NULL, &rx_handle));

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(44100),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = I2S_BCK_PIN,
            .ws = I2S_WS_PIN,
            .dout = I2S_GPIO_UNUSED,
            .din = I2S_DATA_PIN,
            .invert_flags = { false, false, false },
        },
    };

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_handle, &std_cfg));
    ESP_ERROR_CHECK(i2s_channel_enable(rx_handle));
    return inputFile;
}

void setUpWifi() {
    Serial.print("Connecting to Wi-Fi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nWi-Fi connected.");
}

void setUpSD() {
    Serial.print("Initializing SD card");
    while (!SD.begin(5)) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nSD card initialized.");
}

WiFiClient waitForConnection() {
    Serial.print(WiFi.localIP());
    Serial.print(" | Waiting for Connection");
    WiFiClient client;
    while (!client) {
        Serial.print(".");
        delay(500);
        client = server.available();
    }
    Serial.println("\nClient connected.");
    return client;
}

void sendMessage() {
    WiFiClient client = waitForConnection();

    File inputFile = SD.open(inputFileName, FILE_READ);
    while (!inputFile) {
        Serial.println("Failed to open WAV file.");
        inputFile = SD.open(inputFileName, FILE_READ);
        delay(500);
    }

    Serial.print("Sending");
    while (inputFile.available()) {
        Serial.print(".");
        char buffer[512];
        size_t bytesRead = inputFile.readBytes(buffer, sizeof(buffer));
        client.write((uint8_t*)buffer, bytesRead);
    }

    inputFile.close();
    client.stop();
    Serial.println("\nFile Sent");
}

void getResponse() {
    WiFiClient client = waitForConnection();

    while (!client.available() && client.connected()) {
        Serial.print(".");
        delay(500);
    }

    File outputFile = SD.open(outputFileName, FILE_WRITE);
    while (!outputFile) {
        Serial.println("\nFailed to open WAV file.");
        outputFile = SD.open(outputFileName, FILE_WRITE);
        delay(500);
    }

    Serial.println("Receiving");
    while (client.connected()) {
        while (client.available()) {
            Serial.print(".");
            char buffer[512];
            int bytesRead = client.read((uint8_t*)buffer, sizeof(buffer));
            outputFile.write((uint8_t*)buffer, bytesRead);
        }
    }

    outputFile.close();
    client.stop();
    Serial.println("\nFile received and saved.");
}

void setup() {
    Serial.begin(115200);
    delay(5000);
    setUpWifi();
    setUpSD();
    pinMode(34, INPUT);
    server.begin();
    Serial.println("Server started.");
}

void checkHeap() {
    if (heap_caps_check_integrity_all(true)) {
        Serial.println("Heap is healthy.");
    } else {
        Serial.println("Heap is fragmented or corrupted.");
    }
}

bool recorded = false;

void loop() {
    int inputState = digitalRead(34);
    if (inputState == HIGH && recorded == false) {
        File inputFile = setupI2SMic();
        int i = 0;
        Serial.println("Recording...");
        while (true) {
            static uint32_t dataSize = 0;
            uint8_t buffer[256];
            size_t bytesRead;

            ESP_ERROR_CHECK(i2s_channel_read(rx_handle, buffer, sizeof(buffer), &bytesRead, portMAX_DELAY));
            inputFile.write((uint8_t*)buffer, bytesRead);
            dataSize += bytesRead;

            inputState = digitalRead(34);
            i++;

            if (inputState != HIGH) {
                inputFile.seek(0);
                writeWavHeader(inputFile, 44100, dataSize);
                inputFile.close();
                Serial.println("Recording complete.");
                recorded = true;
                i2s_channel_disable(rx_handle);
                i2s_del_channel(rx_handle);
                break;
            }
        }

        if (i > 1000) {
            sendMessage();
        } else {
            Serial.println("Message too short");
            recorded = false;
        }
    }

    if (recorded) {
        getResponse();
        Audio audio;
        audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
        audio.setVolume(16); // 0 ... 21
        audio.connecttoFS(SD, outputFileName);

        while (recorded) {
            audio.loop();

            if (!audio.isRunning()) {
                Serial.println("Playback complete.");
                recorded = false;
            }
        }
    }
}

