#include "_littlefs.h"

void writeFile(const char * path, const char * message) {
  Serial1.printf("Writing file: %s\n", path);

  File file = LittleFS.open(path, "w");
  if (!file) {
    Serial1.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial1.println("File written");
  } else {
    Serial1.println("Write failed");
  }
  delay(200); // Make sure the CREATE and LASTWRITE times are different
  file.close();
}


String readFile(const char * path) {
  Serial1.printf("Reading file: %s\n", path);

  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial1.println("Failed to open file for reading");
  }

  Serial1.print("Read from file: ");
  String data;

  while (file.available()) {
    data += file.readString();

    // Serial.write(file.read());
  }
  // Serial.println(data);
  file.close();
  return data;
}

void initializeLittleFS() {
    if(!LittleFS.begin(true)){
        Serial1.println("LITTLEFS Mount Failed");
        return;
    }
}