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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "util/Filesystem.h"

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

    FileSystemHandler filesystem;
    std::string defaultConfFile = filesystem.expand_user("~/.bmfec/bmfec.conf");

    filesystem.CreateFile(defaultConfFile); // will fail if the file already exists

    try {

        po::options_description desc("Default options");
        desc.add_options()
                ("config,c", po::value<std::string>(&configpath)->default_value(defaultConfFile),"Configuration file, default ~/.bmfec/bmfec.conf")
                ("help,h", "display this help text")
                ("version,v", "display version number")
                ;

        po::options_description network("Network options");
        network.add_options()
                //("network.netmodule", po::value<std::string>()->default_value("bitmessage")->notifier(boost::bind(&MainConfigParser::setNetworkModule, this, _1)),"Net Communication Module")
                ("network.defaultEnableOutbound", po::value<bool>(&m_defaultEnableOutbound)->default_value(false), "Enable outbound communications on startup")
                ("network.loadModuleOnStart", po::value<bool>(&m_loadModuleOnStart)->default_value(false), "Load API handlers on startup")
                ("network.polltime", po::value<int>(&m_pollTime)->default_value(10), "Poll for data every X seconds (between 1 and 60)")


                ("network.bitmessage.remotehost", po::value<std::string>(&remote_bitmessagehost)->default_value("localhost"), "Remote Bitmessage API Host")
                ("network.bitmessage.remoteport", po::value<int>(&remote_bitmessageport)->default_value(8442), "Remote Bitmessage API server port")
                ("network.bitmessage.remoteuser", po::value<std::string>(&remote_bitmessageuser)->default_value("apiuser"), "Remote Bitmessage API Username")
                ("network.bitmessage.remotepass", po::value<std::string>(&remote_bitmessagepass)->default_value("apipass"), "Remote Bitmessage API Password")

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


void MainConfigParser::writeConfigFile(std::string file){

    FileSystemHandler filesystem;
    std::string confFile;

    if(file == ""){
        confFile = filesystem.expand_user("~/.bmfec/bmfec.conf");
    }
    else{
        confFile = file;
    }

    boost::property_tree::ptree root;
    boost::property_tree::ptree network;
    network.put("defaultEnableOutbound", m_defaultEnableOutbound);
    network.put("loadModuleOnStart", m_loadModuleOnStart);
    network.put("polltime", m_pollTime);
    root.push_front(
            boost::property_tree::ptree::value_type( "network", network )
    );

    boost::property_tree::ptree networkBitMessage;
    networkBitMessage.put("remotehost", remote_bitmessagehost);
    networkBitMessage.put("remoteport", remote_bitmessageport);
    networkBitMessage.put("remoteuser", remote_bitmessageuser);
    networkBitMessage.put("remotepass", remote_bitmessagepass);

    root.push_front(
            boost::property_tree::ptree::value_type( "network.bitmessage", networkBitMessage )
    );


    boost::property_tree::write_ini(confFile, root);

}



