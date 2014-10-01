#include <iostream>
#include <locale.h>
#include <string>

#include "engine/shell.h"
#include "parsers/MainConfigParser.h"
#include "util/logger.h"
#include "util/term_control.h"
#include "util/Filesystem.h"

#include "managers/NetworkManager.h"



#ifdef __APPLE__
extern "C" int main(int argc, char **argv){
#else
    int main(int argc, char **argv){
#endif

    GlobalLogger::Instance()->writeToLogFile("debug.log", "Starting");

    setlocale(LC_ALL, "en_US.UTF-8");

    std::cout << "Please standby while bmfec-client is initialized" << std::endl;
    sleep(2);
    term_clear();

    FileSystemHandler fsHandler;
    fsHandler.CreateDirectory("~/.bmfec");

    MainConfigParser::Instance()->parse(argc, argv);

    _SharedPtr<Shell>shell(new Shell());

    shell->boot();
    shell->loadInterfaces(shell);
    shell->run();


    while(shell->running()){

    }

    std::cout << "bmfec-client shutting down" << std::endl;
    sleep(1);

    return 0;

}