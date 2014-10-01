//
//  MainConfigParser.cpp
//  bmfec-client
//
//  Yamamushi
//  9/29/2014
//


#include "MainConfigParser.h"
#include "BitMessage.h"
#include "util/version.h"

#include <boost/filesystem.hpp>
#include <boost/progress.hpp>
#include <boost/program_options.hpp>
#include <boost/bind.hpp>

#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <locale>



namespace po = boost::program_options;

using namespace bmwrapper;

MainConfigParser* MainConfigParser::m_pInstance = NULL;

MainConfigParser* MainConfigParser::Instance(){
    if(!m_pInstance)
        m_pInstance = new MainConfigParser;

    return m_pInstance;
}


bool MainConfigParser::parse(int _argc, char* _argv[]){

    argc = _argc;
    argv = _argv;

    try {

        po::options_description desc("Default options");
        desc.add_options()
                ("config,c", po::value<std::string>(&configpath)->default_value("bmfec.conf"),"Configuration file, default bmfec.conf")
                ("help,h", "display this help text")
                ("version,v", "display version number")
                ;

        po::options_description network("Network options");
        network.add_options()
                //("network.netmodule", po::value<std::string>()->default_value("bitmessage")->notifier(boost::bind(&MainConfigParser::setNetworkModule, this, _1)),"Net Communication Module")
                ("network.defaultEnableOutbound", po::value<bool>(&m_defaultEnableOutbound)->default_value(false), "Enable outbound communications on startup")
                ("network.loadModuleOnStart", po::value<bool>(&m_loadModuleOnStart)->default_value(false), "Load API handlers on startup")


                ("network.bitmessage.remotehost", po::value<std::string>(&remote_bitmessagehost)->default_value("localhost"), "Remote Bitmessage API Host")
                ("network.bitmessage.remoteport", po::value<int>(&remote_bitmessageport)->default_value(8442), "Remote Bitmessage API server port")
                ("network.bitmessage.remoteuser", po::value<std::string>(&remote_bitmessageuser)->default_value("defaultuser"), "Remote Bitmessage API Username")
                ("network.bitmessage.remotepass", po::value<std::string>(&remote_bitmessagepass)->default_value("defaultpass"), "Remote Bitmessage API Password")
                ("network.bitmessage.polltime", po::value<int>(&m_pollTime)->default_value(10), "Poll for data every X seconds (between 1 and 60)")

                ;


        po::options_description config_file_options;
        config_file_options.add(network);

        po::variables_map vm;

        //  Parse our command line options
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << global_version_string << std::endl;
            std::cout << desc << std::endl;
            std::cout << network << std::endl;
            exit(0);
        }
        if (vm.count("version")) {
            std::cout << global_version_string << std::endl;
            exit(0);
        }


        // Now Parse our config file

        std::ifstream ifs(configpath.c_str());
        if(!ifs)
        {
            std::cout << global_version_string << std::endl;
            std::cout << std::endl;
            //std::cout << "Usage:" << std::endl;
            std::cout << desc << std::endl;
            std::cout << std::endl;
            std::cout << "Error: could not open config file \"" << configpath << "\"" << std::endl;

            exit(0);
        }
        else
        {
            store(parse_config_file(ifs, config_file_options), vm);
            notify(vm);
        }

        if(m_pollTime > 60 || m_pollTime < 0){
            m_pollTime = 10;
        }

    }

    catch(std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        exit(1);
    }
    catch(...) {
        std::cerr << "Exception of unknown type!\n";
        return false;
    }
    return true;
}



