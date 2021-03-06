//
//  NcursesManager.cpp
//  bmfec-client
//
//  Created by Jonathan Rumion on 9/7/14.
//  Copyright (c) 2014 TAP. All rights reserved.
//

#include "NcursesManager.h"

#ifdef _BMFEC_NCURSESCLIENT_
#include "util/term_control.h"
#include "util/Clock.h"
#include <iostream>


bool NcursesManager::start(){

    initscr();
    refresh();
    
    if(has_colors() == FALSE)
	{
        endwin();
        std::cout << "Your terminal does not support color\n" << std::endl;
		return false;
	}
    
    getmaxyx(stdscr,m_rows,m_cols);
    
    
    raw(); // disable line buffering
    cbreak();			// Line buffering disabled, Pass on
    noecho();			/* Don't echo() while we do getch */
    start_color();			/* Start color 			*/
    curs_set(0);
    refresh();
    
    m_inputInitialized = false;
    
    
    // This is about to get fun
    // Enable non-blocking input
    keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
    nodelay(stdscr, true);
    std::cout << std::string() << std::flush;
    return true;
    
}

int NcursesManager::getfps(){
    
    int fps = FpsCounter::Instance()->get();

    return fps;
    
}

int NcursesManager::getWidth(){
    
    getmaxyx(stdscr,m_rows,m_cols);
    return m_cols;
    
}

int NcursesManager::getHeight(){
    getmaxyx(stdscr,m_rows,m_cols);
    return m_rows;
}

void NcursesManager::setmaxfps(int fps){
    
        if(fps < 1)
            fps = 1;

        if(fps > 1000)
            fps = 1000;

        m_maxfps = fps;

        timeout(1000/m_maxfps);

}



_SharedPtr<GenericWindow> NcursesManager::getNewWindow(){
    
    
    return _SharedPtr<NcursesWindow>(new NcursesWindow(getHeight(), getWidth(), 0, 0));
    
}



int NcursesManager::getInput(){

    return getch();
    
}




void NcursesManager::shutdown(){
    
    endwin();

}


void NcursesManager::startRawInputFeed() {

    GraphicsDriverManager::startRawInputFeed();

}

void NcursesManager::stopRawInputFeed() {

    GraphicsDriverManager::stopRawInputFeed();

}



#endif