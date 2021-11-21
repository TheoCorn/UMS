
#ifndef spiffs_h
#define spiffs_h

#include "FS.h"
#include "SPIFFS.h"
#include <vector>
#include "Error.h"
#include "sysInfo.h"

namespace spiffs {


/**
 * @param fs
 * @param path
 * @return char*
 */
uint8_t *readFile(fs::FS &fs, const char *path);

/**
  *
  * @param fs
  * @param dirname
  * @return std::vector<Files>
  */
std::vector<fs::File> listDir(fs::FS &fs, char * dirname);


/**
 *
 * @param fs
 * @param path
 * @param value
 */
void writeFile(fs::FS &fs, const char *path, const char* value);

}


#endif
