#ifndef APPLICATION_H
#define APPLICATION_H

#include "ImageMap.h"

#include <list>
#include <string>

using namespace std;

enum image_t {PNG, JPG, GIF, UNSUPPORTED};

class Application {
 public:
    Application(int argc, char *argv[]);
    void ParseArgs(int argc, char *argv[]);
    void PrintUsage();
    void PrintHelp();
    void WalkTrees();
    void WalkTree(string dir_name);
    void AddSupportedImage(string filename);
    image_t DetectFileType(string filename);
    void CreateMaps();
    string IntToString(int num);
    void CreateXHTML();
    list<string> images;

 private:
    list<string> directories;
    list<ImageMap> image_maps;
};

#endif /* APPLICATION_H */
