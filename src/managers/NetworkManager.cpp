#include"NetworkManager.h"

#include "tr1/tr1_threading.h"
#include "tr1/tr1_wrapper.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "util/Clock.h"

#include <BitMessage.h>


NetworkManager* NetworkManager::m_pInstance = NULL;


NetworkManager *NetworkManager::Instance() {

    if (!m_pInstance)   // Only allow one instance of class to be generated.
        m_pInstance = new NetworkManager;

    return m_pInstance;
}


void NetworkManager::startModule(std::string const module, std::string const commstring) {

    std::string netmodule = module;

    // Convert to lower case, take into account locale (will construct a locale object representing your preferred locale)
    std::transform(netmodule.begin(), netmodule.end(), netmodule.begin(), std::bind2nd(std::ptr_fun(&std::tolower<char>), std::locale("")));

    if(netmodule == "bitmessage"){

        for(size_t x = 0; x < m_modulesList.size(); x++){
            if(m_modulesList.at(x)->getCommstring() == commstring && m_modulesList.at(x)->moduleType() == ModuleType::BITMESSAGE){
                return;
            }
        }

        _SharedPtr<NetworkModule> l_bmModule(new bmwrapper::BitMessage(commstring));
        m_modulesList.push_back(l_bmModule);

    }
    else{
        std::cout << std::endl << "Unable to initialize Unknown Network Module: \"" << netmodule << "\"" << std::endl;
    }

}

int NetworkManager::modulesLoaded() {
    if(m_modulesList.size() > 0 )
        return m_modulesList.at(0)->modulesLoaded();
    else
        return 0;
}

int NetworkManager::modulesAlive() {
    if(m_modulesList.size() > 0 )
        return m_modulesList.at(0)->modulesAlive();
    else
        return 0;
}

bool NetworkManager::moduleAccessible(std::string moduleName) {

    if(m_modulesList.size() == 0 )
        return false;

    else{
        std::transform(moduleName.begin(), moduleName.end(), moduleName.begin(), std::bind2nd(std::ptr_fun(&std::tolower<char>), std::locale("")));
        ModuleType l_modType;
        if(moduleName == "bitmessage"){
            l_modType = ModuleType::BITMESSAGE;
        }
        else{
            l_modType = ModuleType::BITMESSAGE;
        }
        for(size_t x = 0; x < m_modulesList.size(); x++){
            if(m_modulesList.at(x)->moduleType() == l_modType){
                //m_modulesList.at(x)->pollStatus();
                return m_modulesList.at(x)->accessible();
            }
        }

        return false;

    }
}


void NetworkManager::stopNetwork(){

    for(size_t x = 0; x < m_modulesList.size(); x++){
        m_modulesList.at(x)->stopQueue();
    }

}