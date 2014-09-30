#pragma once
//
//  connectionwidget.h
//  bmfec-client
//
//  Created by Jonathan Rumion on 8/30/14.
//  Copyright (c) 2014 TAP. All rights reserved.
//


#include "widget.h"
#include "wintypes/window.h"


class GenericWindow;

class ConnectionWidget: public Widget {
    
public:
    
    ConnectionWidget(_SharedPtr<GenericWindow> parent, int xpos, int ypos) : Widget(parent){};
    
    void render();
    void refresh();
    void resize(int newx, int newy);
    void handleKeys(int input);
    
    void hide(){m_hidden = true;}
    void show(){m_hidden = false;}
    void toggleHidnde(){m_hidden = !m_hidden;}
    
protected:
    
    bool m_hidden;
    
};

