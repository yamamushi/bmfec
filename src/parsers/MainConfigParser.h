#pragma once
//
//  MainConfigParser.h
//
//  bmfec-client
//  Yamamushi 9/29/2014
//


#include <string>
#include <memory>

#include "Network.h"

const std::string g_versionString("Auditor 0.0.0");

class MainConfigParser {

public:

    static MainConfigParser* Instance();

    bool parse(int _argc, char* _argv[]);

    std::string getRemote_bitmessageport() {
        return std::to_string(remote_bitmessageport);
    }

    std::string getRemote_bitmessagehost() {
        return remote_bitmessagehost;
    }

    std::string getRemote_bitmessageuser() {
        return remote_bitmessageuser;
    }

    std::string getRemote_bitmessagepass(){
        return remote_bitmessagepass;
    }

    int getPollTime(){return m_pollTime;}

    bool getDefaultEnabledOutbound(){return m_defaultEnableOutbound;}

    bool getLoadModuleOnStart(){return m_loadModuleOnStart;}


private:

    // Singleton Data
    MainConfigParser(){};
    MainConfigParser(MainConfigParser const&);
    MainConfigParser& operator=(MainConfigParser const&);
    static MainConfigParser* m_pInstance;

    // Variables

    int argc;
    char** argv;

    std::string configpath;


private:

// Network
    int remote_bitmessageport;
    std::string remote_bitmessagehost;
    std::string remote_bitmessageuser;
    std::string remote_bitmessagepass;

    int m_pollTime;

    bool m_defaultEnableOutbound;
    bool m_loadModuleOnStart;


};