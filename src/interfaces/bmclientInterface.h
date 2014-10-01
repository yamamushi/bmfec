#pragma once
//
//  debugWindow.h
//  bmfec-client
//
//  Created by Jonathan Rumion on 8/29/14.
//  Copyright (c) 2014 TAP. All rights reserved.
//


#include "interface.h"
#include "menu.h"
#include "wintypes/window.h"
#include "util/Clock.h"

#include "widgets/TimeWidget.h"
#include "widgets/ScrollTextWidget.h"

#include "tr1/tr1_wrapper.h"

#include <sstream>

class bmclientInterface : public Interface {

public:

    bmclientInterface(_SharedPtr<Shell> owner);

    void init();
    void run();
    void handleKeys(int input);
    void draw();

protected:

    void resizeComponents();

private:

    _SharedPtr<TimeWidget> timeWidget;
    _SharedPtr<ScrollTextWidget> m_scrollBox;


    void getLineInput();
    void handleLineInput(std::string input);


    std::string m_lineInputString;
    std::vector<std::string> messageList;

    std::ostringstream m_strCout;

    void update();
    unsigned int m_ticks;
    unsigned int m_secondsElapsed;
    Interval m_timeKeeper;

    bool m_networkActive;
    bool m_outboundAllowed;
    int m_pollTime;

    // These functions we use when talking to BitMessage



};


