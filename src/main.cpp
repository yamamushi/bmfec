#include <iostream>
#include <locale.h>
#include <string>

#include "engine/shell.h"
#include "util/logger.h"
#include "util/term_control.h"

#include "managers/NetworkManager.h"



#ifdef __APPLE__
extern "C" int main(int argc, char **argv){
#else
    int main(int argc, char **argv){
#endif

    if(argc < 2){
        std::cout << "Usage: bmfec-client <apipassword>" << std::endl;
        return 0;
    }


    GlobalLogger::Instance()->writeToLogFile("debug.log", "Starting");

    setlocale(LC_ALL, "en_US.UTF-8");

    std::cout << "Welcome to bmfec-client" << std::endl;
    sleep(1);
    std::cout << "Please standby while Network is initialized" << std::endl;
    sleep(1);
    term_clear();

    std::string pw = argv[1];
    std::string commstring("localhost,8442,debuguser," + pw);

    NetworkManager::Instance()->startModule("bitmessage", commstring);


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