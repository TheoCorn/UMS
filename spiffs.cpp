//
// Created by theo on 4/24/2021.
//

#include "spiffs.hpp"
#include "FS.h"
#include "SPIFFS.h"


using namespace spiffs;

/**
 * @param fs
 * @param path
 * @return char*
 */
unsigned char *spiffs::readFile(fs::FS &fs, const char *path) {

    File file = fs.open(path);
    if (!file || file.isDirectory()) return nullptr;


    size_t fileSize = file.size();
    auto *s = new uint8_t[fileSize];
    auto arrayPointer = s;

    while (fileSize) {
        size_t toRead = (fileSize <= 512) ? fileSize : 512;
        file.read(arrayPointer, toRead);

        arrayPointer += toRead;
        fileSize -= toRead;
    }


    file.close();

    return s;

}


using namespace spiffs;

/**
  *
  * @param fs
  * @param dirname
  * @return std::vector<Files>
  */
std::vector<fs::File> spiffs::listDir(fs::FS &fs, char *dirname) {
    std::vector<File> files;
    File root;

    if (dirname == nullptr) {
        root = fs.open("/");
    } else {
        root = fs.open(dirname);
    }

    while (true) {
        File f = root.openNextFile();
        if (!f) break;
        files.emplace_back(f);
    }

    return files;

}
