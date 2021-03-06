#pragma once
/*

NetworkManager.h

  bmfec-client

Yamamushi
9/28/2014

 */

#include <string>
#include <utility>
#include <vector>

#include "util/messagequeue.h"
#include "tr1/tr1_threading.h"
#include "tr1/tr1_wrapper.h"

#include "Network.h"

class NetworkManager {

public:

    static NetworkManager* Instance();

    void startModule(std::string const module, std::string const commstring);

    int  modulesLoaded();
    int  modulesAlive();

    bool moduleAccessible(std::string moduleName);
    void moduleCheckMail(std::string moduleName);
    void checkMail();
    bool moduleNewMail(std::string moduleName);
    bool newMail();

    void stopNetwork();

private:

    NetworkManager(){};  // Private so that it can  not be called
    NetworkManager(NetworkManager const&){};             // copy constructor is private

    // Ignore the compiler warnings here, this is something we want
    // The copy operator should always be private in a singleton!
    NetworkManager& operator=(NetworkManager const&);  // Ignore the compiler! This Assignment operator is private!
    static NetworkManager* m_pInstance;


    std::vector<_SharedPtr<NetworkModule> > m_modulesList;

};
