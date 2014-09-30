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

    std::cout << "Network Module: " << netmodule << std::endl;

    if(netmodule == "bitmessage"){

        _SharedPtr<NetworkModule>(new bmwrapper::BitMessage(commstring));

    }
    else{
        std::cout << std::endl << "Unable to initialize Network Module: \"" << netmodule << "\"" << std::endl;
        std::cout << "Has it been configured properly?" << std::endl << std::endl;
    }

}