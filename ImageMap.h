#ifndef IMAGEMAP_H
#define IMAGEMAP_H

#include <list>
#include <string>
#include <gd.h>

using namespace std;

class ImageMap {
 public:
    ImageMap(string name);
    ~ImageMap();
    void AddImage(string image);
    gdImagePtr LoadImage(string file);
    void Save();
    string xhtml;
    string name;

 private:
    int counter;
    list<string> images;
    gdImagePtr image;
};

#endif /* IMAGEMAP_H */
