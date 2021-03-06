#pragma once
//
//  windowthread.h
//  bmfec-client
//
//  Created by Jonathan Rumion on 9/6/14.
//  Copyright (c) 2014 TAP. All rights reserved.
//


#include "tr1/tr1_wrapper.h"
#include "tr1/tr1_threading.h"

#include "window.h"
#include "windowthread.h"


// The idea is not to stop other threads from writing to the window completely
// But to provide a mechanism for windows to be written faster to the screen


class MainWindowThread
{
    
public:
    
    static MainWindowThread* Instance();
    
    void start();
    void stop();
    int getfps();
    
    
    
protected:

    void run();
    
    
private:
    
    // Constructor
    MainWindowThread();
    
    MainWindowThread(MainWindowThread const&){};
    MainWindowThread& operator=(MainWindowThread const&);
    
    static MainWindowThread* m_pInstance;
    
    bool m_started;
    
    _SharedPtr<GenericWindow> m_window;
    
    _THREAD m_thread;
    _MUTEX(m_processing);
    CONDITION_VARIABLE(m_conditional);
    
    _ATOMIC(m_working);
    
};



