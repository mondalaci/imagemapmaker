#include "ImageMap.h"
#include "Config.h"
#include "Application.h"
#include <iostream>

extern Config config;
extern Application *application;

ImageMap::ImageMap(string name)
{
    this->name = name;
    counter = 0;

    int cell_size = config.image_size + 2*config.image_map_padding + 1;
    int width = config.image_map_width * cell_size + 1;
    int height = config.image_map_height * cell_size + 1;

    gdImagePtr im = gdImageCreateTrueColor(width, height);

    int white = gdImageColorAllocate(im, 255, 255, 255);
    int black = gdImageColorAllocate(im, 0, 0, 0);

    gdImageFilledRectangle(im, 0, 0, width-1, height-1, white);
    gdImageRectangle(im, 0, 0, width-1, height-1, black);

    for (int i=0; i<height; i++) {
        gdImageLine(im, 0, i*cell_size, width-1, i*cell_size, black);
    }

    for (int i=0; i<width; i++) {
        gdImageLine(im, i*cell_size, 0, i*cell_size, height-1, black);
    }

    image = im;

    xhtml += "<img src=\"" + name + ".png\" usemap=\"#" + name + "\" border=\"0\"/>\n";
    xhtml += "<map name=\"" + name + "\">\n";
}

ImageMap::~ImageMap()
{
}

void ImageMap::AddImage(string image)
{
    int vertical_idx = counter / config.image_map_width;
    int horizontal_idx = counter % config.image_map_width;

    int cell_pos_x = horizontal_idx *
        (config.image_size + 2*config.image_map_padding + 1) + 1;
    int cell_pos_y = vertical_idx *
        (config.image_size + 2*config.image_map_padding + 1) + 1;

    xhtml += "  <area shape=\"rect\" coords=\"" + application->IntToString(cell_pos_x);
    xhtml += "," + application->IntToString(cell_pos_y);
    xhtml += "," + application->IntToString(cell_pos_x+config.image_size+2*config.image_map_padding);
    xhtml += "," + application->IntToString(cell_pos_y+config.image_size+2*config.image_map_padding);
    xhtml += "\" href=\"" + image + "\" />\n";

    gdImagePtr im = LoadImage(image);
    gdImageCopyResized(this->image, im,
                       cell_pos_x+config.image_map_padding, cell_pos_y+config.image_map_padding,
                       0, 0,
                       config.image_size, config.image_size,
                       im->sx, im->sy);

    images.push_back(image);
    gdImageDestroy(im);

    counter++;
}

gdImagePtr ImageMap::LoadImage(string file)
{
    gdImagePtr im;
    FILE* fp;
    fp = fopen(file.c_str(), "r");

    switch (application->DetectFileType(file)) {
    case PNG:
        im = gdImageCreateFromPng(fp);
        break;
    case JPG:
        im = gdImageCreateFromJpeg(fp);
        break;
    case GIF:
        im = gdImageCreateFromGif(fp);
        break;
    }

    fclose(fp);
    return im;
}

void ImageMap::Save()
{
    string filename = name + ".png";
    FILE *fp = fopen(filename.c_str(), "w");
    gdImagePng(image, fp);
    fclose(fp);
    gdImageDestroy(image);
    xhtml += "</map>\n";

}
