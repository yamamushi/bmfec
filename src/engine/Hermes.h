#pragma once
//
//  Hermes.h
//  bmfec-client
//
//  Created by Jonathan Rumion on 9/6/14.
//  Copyright (c) 2014 TAP. All rights reserved.
//


#include "tr1/tr1_threading.h"
#include "util/messagequeue.h"
#include "network/NetworkMessage.h"
#include "util/logger.h"

class Hermes
{
    
public:
    
    static Hermes* Instance();
    
    
    // Command Queue
    void addToCommandQueue(_STD_FUNCTION(void()) command);
    bool processingCommand();

    
    // Network Message Queue
    void addToNetworkQueue(NetworkMessage message);
    bool processingNetworkQueue();

    
protected:
    
    void runCommandQueue();
    _ATOMIC(m_commandQueuerunning);

    int getCommandQueueSize();
    void clearCommandQueue();
    
    void runNetworkQueue();
    _ATOMIC(m_networkQueuerunning);
    
    int getNetworkQueueSize();
    void clearNetworkQueue();
    
    
private:
    
    // Constructor
    Hermes();
    
    Hermes(Hermes const&){};
    Hermes& operator=(Hermes const&); //Ignore the compiler! This Assignment operator is private!
    
    static Hermes* m_pInstance;

    //bool m_started;
    
    // Command Queue Thread Things
    void startCommandQueue();
    void stopCommandQueue();
    
    bool parseNextCommand();
    
    MsgQueue<_STD_FUNCTION(void())> CommandQueue;
    
    _THREAD m_commandQueuethread;
    _MUTEX(m_commandQueueprocessing);
    CONDITION_VARIABLE(m_commandQueueconditional);
    _ATOMIC(m_commandQueueworking);
    
    
    // Network Queue Thread Things
    void startNetworkQueue();
    void stopNetworkQueue();
    
    bool parseNextNetworkMessage();
    
    MsgQueue<NetworkMessage> NetworkQueue;
    
    _THREAD m_networkQueuethread;
    _MUTEX(m_networkQueueprocessing);
    CONDITION_VARIABLE(m_networkQueueconditional);
    _ATOMIC(m_networkQueueworking);
    
    
};



