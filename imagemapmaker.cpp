#include "Application.h"

Application *application;

int main(int argc, char *argv[], char *envp[])
{
    application = new Application(argc, argv);
}
