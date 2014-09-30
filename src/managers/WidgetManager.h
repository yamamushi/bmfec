#pragma once
//
//  WidgetManager.h
//  bmfec-client
//
//  Created by Jonathan Rumion on 9/5/14.
//  Copyright (c) 2014 TAP. All rights reserved.
//


#include "interfaces/interface.h"
#include "widgets/widget.h"
#include "wintypes/window.h"


#include <vector>
#include <string>

#include "tr1/tr1_wrapper.h"

class Interface;

class WidgetManager {
    
public:
    
    WidgetManager(Interface *owner);
    
    
    void addWidget(_SharedPtr<Widget> target);
    void removeWidget(_SharedPtr<Widget> target);
    
    void selectWidget(_SharedPtr<Widget> target);
    
    void handleKeys(int input);

    _SharedPtr<Widget> getWidgetForID(int ID);
    _SharedPtr<Widget> getPrevWidget();
    _SharedPtr<Widget> getNextWidget();
    _SharedPtr<Widget> getActiveWidget();

    int getActiveWidgetID(){return m_activeWidgetID;}
    int getActiveWidgetIndex(){return m_activeWidgetIndex;}

    void render();


private:
    
    
    std::vector<_SharedPtr<Widget> > m_widgetList;
    
    _SharedPtr<Widget> m_activeWidget;
    
    _SharedPtr<GenericWindow> m_mainWindow;
    Interface *m_owner;

    int m_activeWidgetID;
    int m_activeWidgetIndex;

};


