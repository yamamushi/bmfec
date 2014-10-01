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


    setlocale(LC_ALL, "en_US.UTF-8");

    std::cout << "Please standby while bmfec-client is initialized" << std::endl;
    sleep(2);
    term_clear();

    FileSystemHandler fsHandler;
    if(!fsHandler.CheckIfExists("~/.bmfec"))
        if(!fsHandler.CreateDirectory("~/.bmfec")){
            std::cerr << "Error creating directory ~/.bmfec please check your local file permissions" << std::endl;
            return 0;
        }




    GlobalLogger::Instance()->writeToLogFile("Starting");


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