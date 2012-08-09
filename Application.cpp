#include "Application.h"
#include "Config.h"

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fstream>

extern Config config;

Application::Application(int argc, char* argv[])
{
    ParseArgs(argc, argv);
    WalkTrees();
    CreateMaps();
    CreateXHTML();
}

void Application::ParseArgs(int argc, char *argv[])
{
    AddSupportedImage("df");
    if (argc == 1) {
        PrintUsage();
        exit(0);
    }

    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        PrintHelp();
    }

    for (int i=1; i<argc; i++) {
        if (argv[i][0] != '-') {
            directories.push_back(argv[i]);
        } else {
            if (i+1>=argc) {
                cout << "The argument of option `" << argv[i] <<
                    "' is missing." << endl;
                cout << "Use -h for help." << endl;
                exit(1);
            }

            for (int j=0; j<strlen(argv[i+1]); j++) {
                if (!('0' <= argv[i+1][j] && argv[i+1][j] <= '9')) {
                    cout << "argument `" << argv[i+1] << "' of option `" <<
                        argv[i] << "' is not a valid nonnegative decimal." <<
                        endl;
                    cout << "Use -h for help." << endl;
                    exit(1);
                }
            }

            if (config.ParseArg(argv[i], atoi(argv[i+1]))) {
                cout << "Option `" << argv[i] << "' is invalid." << endl;
                cout << "Use -h for help." << endl;
            }
            i++;
        }
    }

}

void Application::PrintUsage()
{
    cout << "Usage: imagemapmaker [OPTION]... [DIRECTORY]..." << endl;
    cout << "Use -h or --help for detailed options." << endl;
}

void Application::PrintHelp()
{
    PrintUsage();
    cout << endl;
    cout << "Options:" << endl;
    cout << "  -s, --image-size" << endl;
    cout << "    Set the size of the individual images." << endl;
    cout << "    (both horizontal and vertical values)" << endl;
    cout << "    default: 100" << endl;
    cout << "  -p, --image-map-padding" << endl;
    cout << "    Set the padding of the individual cells in pixels." << endl;
    cout << "    default: 5" << endl;
    cout << "  -w, --image-map-width" << endl;
    cout << "    Set the with of the image map (number of images)." << endl;
    cout << "    default: 8" << endl;
    cout << "  -e, --image-map-height" << endl;
    cout << "    Set the height of the image map (number of images)." << endl;
    cout << "    default: 8" << endl;
}

void Application::WalkTrees()
{
    cout << "Searching for images..." << endl;

    list<string>::iterator i;
    for (i=directories.begin(); i!=directories.end(); i++) {
        WalkTree(*i);
    }

    cout << images.size() << " images found." << endl;

    if (images.size() == 0) {
        cout << "Exiting." << endl;
        exit(0);
    }
}

void Application::WalkTree(string dir_name) {
    DIR *dir = opendir(dir_name.c_str());
    struct dirent *entry;
    struct stat *st;
    char str[256];
    st = (struct stat*)malloc(sizeof(struct stat));

    while ((entry=readdir(dir)) != NULL) {
        string current_dir = dir_name + "/" + entry->d_name;

        if ((strcmp(entry->d_name, ".")==0) || (strcmp(entry->d_name, "..")==0)) {
            continue;
        }

        const char *s = current_dir.c_str();
        strncpy(str, s, 256);
        if (access(s, R_OK)) {
            continue;
        }

        if (lstat(str, st)) {
            continue;
            }

        if (S_ISDIR(st->st_mode)) {
            WalkTree(current_dir);
        } else if (S_ISREG(st->st_mode)) {
            AddSupportedImage(current_dir);
        } else {
            continue;
        }
    }


    closedir(dir);
}

void Application::AddSupportedImage(string filename)
{
    string format;

    switch (DetectFileType(filename)) {
    case PNG:
        format = "PNG";
        break;
    case JPG:
        format = "JPG";
        break;
    case GIF:
        format = "GIF";
        break;
    default:
        return;
    }

    images.push_back(filename);
    cout << "Added " << format << ": " << filename << endl;

}

image_t Application::DetectFileType(string filename)
{
    // split the extension part
    int dotpos = filename.rfind(".");
    if (dotpos >= filename.length()) {
        return UNSUPPORTED;
    }

    string ext = filename.substr(dotpos+1, filename.length());

    // lowercase the extension
    for (int i=0; i<ext.length(); i++) {
        if (isupper(ext[i])) {
            ext[i] = tolower(ext[i]);
        }
    }

    // return the detected file type
    if (ext == "png") {
        return PNG;
    }else if (ext == "jpg" || ext == "jpeg") {
        return JPG;
    } else if (ext == "gif") {
        return GIF;
    } else {
        return UNSUPPORTED;
    }
}

void Application::CreateMaps()
{
    int map_space = config.image_map_width * config.image_map_height;
    int map_num = images.size() / map_space + 1;

    list<string>::iterator images_iter = images.begin();

    int image_num = 1;
    for (int map_idx=1; map_idx<=map_num; map_idx++) {
        cout << "Creating map " << map_idx << " of " << map_num << "..." << endl;
        ImageMap *map = new ImageMap("map" + IntToString(map_idx));
        for (int image_idx=0; image_idx<map_space &&
                 images_iter!=images.end(); image_idx++, images_iter++) {

            cout << "Processing Image " << image_num++ << " of " << images.size() << "... ";
            cout.flush();
            map->AddImage(*images_iter);
            cout << "done" << endl;
        }

        map->Save();
        image_maps.push_back(*map);
        cout << "Map " << map_idx << " successfully created." << endl;
    }
}

string Application::IntToString(int num)
{
    char str[20];
    sprintf(str, "%i", num);
    return string(str);
}

void Application::CreateXHTML()
{
    list<ImageMap>::iterator i;
    string xhtml;

    fstream filestr("index.html", ios_base::out);

    filestr << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" "
            << "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n";
    filestr << "<html xmlns=\"http://www.w3.org/1999/xhtml\""
            << "xml:lang=\"en\" lang=\"en\">\n";
    filestr << "<head>";
    filestr << "<meta http-equiv=\"content-type\""
            <<" content=\"text/html; charset=utf-8\"/>\n";
        filestr << "<title>Image Maps</title>\n";
    filestr << "</head>\n";
    filestr << "<body>\n";
    filestr << "<center><h1>\n";
    for (i=image_maps.begin(); i!=image_maps.end(); i++) {
        filestr << i->name << "<br />\n";
        filestr << i->xhtml;
    }

    filestr << "</h1></center>\n";
    filestr << "</body>\n";
    filestr << "</html>\n";


    filestr.close();
}
