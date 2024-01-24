#include <Wire.h>               // I2C library
#include <AudioGeneratorMP3.h>  // library for mp3 decode
#include <AudioOutputI2S.h>     // I2S configuration library
#include <AudioFileSourceSD.h>  // library with ready-to-use function for file reading and etc

// DA converter pins
#define I2S_WS 26
#define I2S_BCK 27
#define I2S_DO 25
#define SD_CS 5 // pin for SD card
#define RATE 44100

const byte AmountOfFiles        = 14;
const char files[AmountOfFiles] = {'D', 'R', 'U', 'M', 'F', '6', '9', 'N', 'I', 'G', 'B', 'A', 'S', 'E'};
const int I2C_address           = 18; // Arduino address

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S    *out;
bool needToPlay = false;

char fileName[] = "/Z.mp3\0";

// test if FIle is accessible
int testFileO(fs::FS &fs, const char *path) {
	File file = fs.open(path);
	static uint8_t buf[101];
	size_t len = 0;
	if (file) {
		len         = 1;
		size_t flen = len;
		while (len != 0) {
			file.read(buf, len--);
		}
		file.close();
		return 0;
	}
	return 1;
}

// function for handling I2C data transmitting
void receiveEvent(int howMany) {
  if(needToPlay)  // if some song still playing, we do not take
    return;
	char songName = '\0'; // basically we get only one letter from transmitting
	while (Wire.available()) { // in case if I2C wire is filled up with some mess, we just skip all to the last character
		songName = Wire.read();
		Serial.print("Get from I2C: ");
		Serial.print(songName);
		Serial.print("\n");
	}
	fileName[1] = songName;
	// check if file exists
	if (testFileO(SD, fileName))
		return;

	delete file;
	file       = new AudioFileSourceSD(fileName);
	needToPlay = true;
}

void setup() {
	delay(2000); // this need to avoid situation when esp32 already send success code, but arduino UNO is not ready to get it
	Serial.begin(115200);
	Serial.print("Begin\n");

	pinMode(SD_CS, OUTPUT);
	digitalWrite(SD_CS, HIGH);

	// setting up card reader
	while (!SD.begin(SD_CS)) {
		Serial.println("SD card is not mounted");
		delay(1500);
	}
	Serial.println("Successfully mounted SD card");
	uint8_t cardType = SD.cardType();
	while (cardType == CARD_NONE) {
		Serial.println("SD card is not attached");
		delay(1000);
	}
	Serial.println("SD card is attached");

	// Initialize I2S
	out = new AudioOutputI2S();
	out->SetPinout(I2S_BCK, I2S_WS, I2S_DO);
	out->SetGain(0.5);

	// Checking files (debug info)
	for (int i = 0; i < AmountOfFiles; i++) {
		fileName[1] = files[i];
		Serial.print(fileName);
		if (testFileO(SD, fileName)) {
			Serial.println(" -- not found");
			continue;
		} else
			Serial.println(" -- found");
	}

	char temp = 'S'; // Success
	Wire.begin();   // joining I2C as master
	Wire.beginTransmission(18); // beginning transmitting with arduino
	Wire.write(temp);
	Wire.endTransmission();
	Serial.print("Success sent\n");
	Wire.end();
	Wire.begin(21); // joining I2C as slave
	Wire.onReceive(receiveEvent); // setting function for handling i2c transmitting
	file = new AudioFileSourceSD(fileName); // dummy file
}

void loop() {
	delay(100);
	if (!needToPlay)
		return;

	// Open MP3 file from SD card
	mp3 = new AudioGeneratorMP3();
	mp3->begin(file, out);

	Serial.println("Playing music...");

	// Play music until the end of the file
	while (mp3->isRunning()) {
		if (!mp3->loop()) {
			mp3->stop();
		}
	}

	Serial.println("Music playback complete");

	// Clean up
	delete mp3;
	needToPlay = false;
}