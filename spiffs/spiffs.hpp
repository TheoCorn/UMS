
#ifndef spiffs_h
#define spiffs_h

#include "FS.h"
#include "SPIFFS.h"
#include <vector>

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



}


#endif
