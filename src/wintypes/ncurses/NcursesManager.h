#pragma once
//
//  NcursesManager.h
//  bmfec-client
//
//  Created by Jonathan Rumion on 9/7/14.
//  Copyright (c) 2014 TAP. All rights reserved.
//


#include "config.h"
#ifdef _BMFEC_NCURSESCLIENT_
#include "managers/GraphicsDriverManager.h"
#include "ncurseswindow.h"
#include <ncurses.h>

class NcursesManager : public GraphicsDriverManager {
    
public:


    bool start();
    
    int getWidth();
    int getHeight();
    int getfps();
    void setmaxfps(int fps);
    
    _SharedPtr<GenericWindow> getNewWindow();

    int getInput();
    
    void startRawInputFeed();
    void stopRawInputFeed();
    
    void shutdown();

    
    
};


#endif