#include "Config.h"

#include <string.h>

Config config;

Config::Config()
{
    image_size = 100;
    image_map_padding = 5;
    image_map_width = 8;
    image_map_height = 8;
}

bool Config::ParseArg(char *arg, int value)
{
    if ((strcmp(arg, "-s") == 0) ||
        (strcmp(arg, "--image-size") == 0)) {
        image_size = value;
        return false;
    }

    else if ((strcmp(arg, "-p") == 0) ||
        (strcmp(arg, "--image-map-padding") == 0)) {
        image_map_padding = value;
        return false;
    }

    else if ((strcmp(arg, "-w") == 0) ||
        (strcmp(arg, "--image-map-width") == 0)) {
        image_map_width = value;
        return false;
    }

    else if ((strcmp(arg, "-e") == 0) ||
        (strcmp(arg, "--image-map-height") == 0)) {
        image_map_height = value;
        return false;
    }

    return true;
}
