#ifndef CONFIG_H
#define CONFIG_H

class Config {
 public:
    Config();
    bool ParseArg(char *arg, int value);

 public:
    int image_size;
    int image_map_padding;
    int image_map_width;
    int image_map_height;
};

#endif /* CONFIG_H */
