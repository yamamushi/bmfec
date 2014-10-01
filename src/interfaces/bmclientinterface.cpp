//
//  debugWindow.cpp
//  bmfec-client
//
//  Created by Jonathan Rumion on 8/29/14.
//  Copyright (c) 2014 TAP. All rights reserved.
//

#include "bmclientInterface.h"
#include "managers/ColorManager.h"
#include "util/version.h"
#include "parsers/MainConfigParser.h"
#include <string>
#include <managers/NetworkManager.h>
#include <iostream>

bmclientInterface::bmclientInterface(_SharedPtr<Shell> owner) : Interface(owner){

}


void bmclientInterface::init(){

    m_ticks = 0;
    m_secondsElapsed = 0;


    // Redirect cerr.
    std::streambuf* oldCerrStreamBuf = std::cerr.rdbuf();

    std::cerr.rdbuf( m_strCout.rdbuf() );

    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf( m_strCout.rdbuf() );

    if(MainConfigParser::Instance()->getLoadModuleOnStart()){

        std::string host = MainConfigParser::Instance()->getRemote_bitmessagehost();
        std::string port = MainConfigParser::Instance()->getRemote_bitmessageport();
        std::string user = MainConfigParser::Instance()->getRemote_bitmessageuser();
        std::string pass = MainConfigParser::Instance()->getRemote_bitmessagepass();

        std::string commstring( host + "," + port + "," + user + "," + pass);
        NetworkManager::Instance()->startModule("bitmessage", commstring);

    }

    m_networkActive = NetworkManager::Instance()->moduleAccessible("bitmessage");
    m_outboundAllowed = MainConfigParser::Instance()->getDefaultEnabledOutbound();
    m_pollTime = MainConfigParser::Instance()->getPollTime();



    m_mainWindow->clearScreen();
    m_mainWindow->hideBorder();

    m_widgetManager = _SharedPtr<WidgetManager>(new WidgetManager(this));

    setName("bmfec-client");

    timeWidget = _SharedPtr<TimeWidget>(new TimeWidget(m_mainWindow, m_width - 28, 1));
    m_widgetManager->addWidget(timeWidget);

    m_scrollBox = _SharedPtr<ScrollTextWidget>(new ScrollTextWidget(m_mainWindow, 1, 4));
    m_scrollBox->resize(m_mainWindow->getX() - 2, m_mainWindow->getY() - 5);
    m_widgetManager->addWidget(m_scrollBox);

    m_scrollBox->addMessage("\n");

    // Needed by our shell controller
    m_initialized = true;

    run();

}


void bmclientInterface::run(){

    update();

    m_mainWindow->clearArea(1, 1, m_width-1, m_height-3);
    m_mainWindow->refresh();
    m_mainWindow->drawAt(1, m_mainWindow->getY()-2, '>');

    //m_activeWidget = m_widgetManager->getActiveWidget();
    m_activeWidget = m_scrollBox;
    timeWidget->move(m_width - 28, 1);

    if(!m_activeWidget)
        return;

    std::string message = m_strCout.str();


    if(message != "") {
        std::string::size_type pos = 0;
        std::string::size_type prev = 0;
        while ((pos = message.find('\n', prev)) != std::string::npos) {
            m_scrollBox->addMessage(message.substr(prev, pos - prev));
            prev = pos + 1;
        }
        m_scrollBox->addMessage("\n");
        m_scrollBox->addMessage("\n");
        //messageList.push_back(message.substr(prev));
        m_strCout.str("");
    }

    m_widgetManager->render();
    draw();

}



void bmclientInterface::handleKeys(int input){

    switch(input){

        case '[':
            m_activeWidget = m_widgetManager->getPrevWidget();
            break;
        case ']':
            m_activeWidget = m_widgetManager->getNextWidget();
            break;

        case '\n':
            getLineInput();
            if(m_lineInputString != ""){

                m_scrollBox->addMessage("-> " + m_lineInputString);
                m_mainWindow->clearLineFrom( m_mainWindow->getY()-2, 2);

                run();
                m_mainWindow->render();

                handleLineInput(m_lineInputString);
                m_lineInputString = "";

            }

            break;
        default:
            m_activeWidget->handleKeys(input);
            break;

    }

}


void bmclientInterface::handleLineInput(std::string input) {

    std::transform(input.begin(), input.end(), input.begin(), ::tolower);

    std::vector<std::string> words;
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;

    words.push_back(" ");
    while ((pos = input.find(' ', prev)) != std::string::npos) {
        words.push_back(input.substr(prev, pos - prev));
        prev = pos + 1;
    }
    words.push_back(input.substr(prev));

    std::string command = words.at(1);

    if(command == "start"){

        if(words.size() < 2 || words.size() > 6) {
            m_scrollBox->addMessage("\n");
            m_scrollBox->addMessage("Usage: start <username> <password> <host> <port>");
            m_scrollBox->addMessage("\n");
        }
        else{

            m_scrollBox->addMessage("\n");



                std::string host = MainConfigParser::Instance()->getRemote_bitmessagehost();
                std::string port = MainConfigParser::Instance()->getRemote_bitmessageport();
                std::string user = MainConfigParser::Instance()->getRemote_bitmessageuser();
                std::string pass = MainConfigParser::Instance()->getRemote_bitmessagepass();

                if(words.size() == 3){
                    user = words.at(2);
                }
                if(words.size() == 4){
                    user = words.at(2);
                    pass = words.at(3);
                }

                if(words.size() == 5) {
                    user = words.at(2);
                    pass = words.at(3);
                    host = words.at(4);
                }

                if(words.size() == 6){
                    user = words.at(2);
                    pass = words.at(3);
                    host = words.at(4);
                    port = words.at(5);
                }

                std::string commstring( host + "," + port + "," + user + "," + pass);

                NetworkManager::Instance()->startModule("bitmessage", commstring);

               m_networkActive = NetworkManager::Instance()->moduleAccessible("bitmessage");

            m_outboundAllowed = true;
            m_scrollBox->addMessage("Outbound Communications Enabled");
            m_scrollBox->addMessage("\n");

        }
    }

    else if(command == "show"){
        m_scrollBox->addMessage("\n");

        if(words.size() < 3){

            m_scrollBox->addMessage("Usage: show <option>");
            m_scrollBox->addMessage("Available Options: config");

        }
        else {
            if(words.at(2) == "config" ) {

                m_scrollBox->addMessage("Option   - Status      | Description");
                m_scrollBox->addMessage("------------------------------------------------------------");

                if (m_outboundAllowed)
                    m_scrollBox->addMessage("outbound - Enabled     | Outbound Communications Enabled");
                else
                    m_scrollBox->addMessage("outbound - Disabled    | Outbound Communications Disabled");

                m_scrollBox->addMessage("polltime - " + std::to_string(m_pollTime) + " seconds  | Poll for data every " + std::to_string(m_pollTime) + " seconds");

            }
            else{
                m_scrollBox->addMessage("Unknown Option: " + words.at(2));
                m_scrollBox->addMessage("Available Options: config");
            }
        }
        m_scrollBox->addMessage("\n");

    }

    else if(command == "set") {
        m_scrollBox->addMessage("\n");

        if (words.size() < 3) {

            m_scrollBox->addMessage("Usage: set <var> <option>");

        }
        else {
            if(words.at(2) == "polltime"){
                if(words.size() < 4){
                    m_scrollBox->addMessage("Usage: set polltime <seconds> - Will default to time set in config file if not a value between 1 and 60");
                }
                else {
                    std::string::const_iterator it = words.at(3).begin();
                    while (it != words.at(3).end() && std::isdigit(*it)) ++it;
                    if (!words.at(3).empty() && it == words.at(3).end()) {
                        std::istringstream convert(words.at(3));
                        if (!(convert >> m_pollTime))
                            m_pollTime = MainConfigParser::Instance()->getPollTime();

                        if (m_pollTime < 1 || m_pollTime > 60)
                            m_pollTime = MainConfigParser::Instance()->getPollTime();

                        m_scrollBox->addMessage("polltime - " + std::to_string(m_pollTime) + " seconds  | Poll for data every " + std::to_string(m_pollTime) + " seconds");

                    }
                    else {
                        m_scrollBox->addMessage("Usage: set polltime <seconds> - Will default to time set in config file if not a value between 1 and 60");
                    }
                }

            }
            else{
                m_scrollBox->addMessage("Unknown Variable: " + words.at(2));
            }
        }

        m_scrollBox->addMessage("\n");

    }


    else if(command == "enable"){
        m_scrollBox->addMessage("\n");

        if(words.size() < 3){
            m_scrollBox->addMessage("Usage: enable <var> <var2> ...");
        }
        else{
            m_scrollBox->addMessage("Option   - Status   | Description");
            m_scrollBox->addMessage("----------------------------------------------------");
            for(size_t x = 2; x < words.size(); x++){
                if(words.at(x) == "outbound") {
                    m_scrollBox->addMessage("outbound - Enabled  | Outbound Communications Enabled");
                    m_outboundAllowed = true;
                }
                else{
                    m_scrollBox->addMessage("Unknown Variable: " + words.at(x));
                }

            }
        }
        m_scrollBox->addMessage("\n");

    }

    else if(command == "disable"){
        m_scrollBox->addMessage("\n");

        if(words.size() < 3){
            m_scrollBox->addMessage("Usage: disable <var> <var2> ...");
        }
        else{
            m_scrollBox->addMessage("Option   - Status   | Description");
            m_scrollBox->addMessage("----------------------------------------------------");
            for(size_t x = 2; x < words.size(); x++){
                if(words.at(x) == "outbound") {
                    m_scrollBox->addMessage("outbound - Disabled | Outbound Communications Disabled");
                    m_outboundAllowed = false;
                }
                else{
                    m_scrollBox->addMessage("Unknown Variable: " + words.at(x));
                }

            }
        }
        m_scrollBox->addMessage("\n");


    }

    else if(command == "stop"){
        m_outboundAllowed = false;
        NetworkManager::Instance()->stopNetwork();
        m_scrollBox->addMessage("\n");
        m_scrollBox->addMessage("Outbound Communications Disabled, this does not guarantee that the associated daemon will halt outbound communications");
        m_scrollBox->addMessage("\n");
    }

    else if(command == "help"){
        m_scrollBox->addMessage("\n");
        m_scrollBox->addMessage("bmfec-client Version: " + global_version_string);
        m_scrollBox->addMessage("\n");
        m_scrollBox->addMessage("Available Commands: ");
        m_scrollBox->addMessage("disable <var> <var2> ... - Disable a list of variables");
        m_scrollBox->addMessage("enable <var> <var2> ... - Enable a list of variables");
        m_scrollBox->addMessage("help - Display this help text");
        m_scrollBox->addMessage("quit - Shutdown bmfec-client");
        m_scrollBox->addMessage("set <var> <option>- Set a given variable");
        m_scrollBox->addMessage("show <option> - Show a given option");
        m_scrollBox->addMessage("start <username> <password> <host> <port> - Load the BitMessage Network Module");
        m_scrollBox->addMessage("stop - Stop outbound communications");
        m_scrollBox->addMessage("version - Display the version");

        m_scrollBox->addMessage("\n");

    }

    else if(command == "version") {
        m_scrollBox->addMessage("\n");
        m_scrollBox->addMessage("bmfec-client Version: " + global_version_string);
        m_scrollBox->addMessage("\n");
    }

    else if(command == "quit"){

        m_owner->quit();

    }

    else if(command == ""){
        return;
    }

    else {
        m_scrollBox->addMessage("\n");
        m_scrollBox->addMessage("Unknown command: " + command);
        m_scrollBox->addMessage("\n");
    }


}


void bmclientInterface::draw(){

    // I prefer writing from top left to right, like you would on a piece of paper.
    if(m_networkActive)
        m_mainWindow->drawAt(10, 1, "BitMessage API Connected");
    else
        m_mainWindow->drawAt(10, 1, "BitMessage API Not Connected", COLOR_RED);

    if(m_outboundAllowed)
        m_mainWindow->drawAt(2, 1, "o");
    else
        m_mainWindow->drawAt(2, 1, "o", COLOR_RED);




    m_mainWindow->drawAt( (m_width - (int)getName().size())/2, 1, getName(), COLOR_WHITE, COLOR_BLACK);
    m_mainWindow->drawAt( (m_width - (int)global_version_string.length())/2, 2, global_version_string, COLOR_WHITE, COLOR_BLACK);
    m_mainWindow->drawLine(1, 3, m_width-1, 3, "-", m_mainWindow->getBorderColorfg(), m_mainWindow->getBorderColorbg());
    m_mainWindow->drawLine(1, m_height-3, m_width-1, m_height-3, "-", m_mainWindow->getBorderColorfg(), m_mainWindow->getBorderColorbg());

};


void bmclientInterface::getLineInput(){

    //m_mainWindow->drawAt(4, m_mainWindow->getY()-2, '|', m_mainWindow->getFGColor(), m_mainWindow->getBGColor(), A_BLINK);
    m_owner->enableRaw();

    while(int input = getch() ){
        m_mainWindow->drawAt(1, m_mainWindow->getY()-2, '>');
        m_mainWindow->drawAt(3 + m_lineInputString.length(), m_mainWindow->getY()-2, '|', m_mainWindow->getFGColor(), m_mainWindow->getBGColor(), A_BLINK);


        if(input == '\n' || input == 27)
            break;

        if(input > 0){

            if(input == KEY_BACKSPACE || input == KEY_DC || input == 127) {
                m_lineInputString.pop_back();
                m_mainWindow->clearLineFrom(m_mainWindow->getY()-2, m_lineInputString.length());
            }
            else
                m_lineInputString += input;

            //m_mainWindow->drawAt((m_mainWindow->getX()/2), (m_mainWindow->getY()/2), "             ");
            //m_mainWindow->clearLineFrom( (m_mainWindow->getY()/2), (m_mainWindow->getX()/2));

            m_mainWindow->drawAt(3, m_mainWindow->getY()-2, m_lineInputString);

        }

        run();
        m_mainWindow->refresh();
        m_mainWindow->render();

        input = 0;
    }

    m_owner->disableRaw();

}



void bmclientInterface::resizeComponents(){



}



void bmclientInterface::update(){

    if(m_timeKeeper.value() > 1000)
    {
        m_secondsElapsed++;

        m_ticks++;
        m_timeKeeper = Interval();
    }
    if(m_ticks == 60){
        m_ticks = 0;
    }


    if(m_ticks % m_pollTime == 0)
        m_networkActive = NetworkManager::Instance()->moduleAccessible("bitmessage");

}