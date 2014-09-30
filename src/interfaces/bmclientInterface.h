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

#include "widgets/widget.h"

#include "tr1/tr1_wrapper.h"

class bmclientInterface : public Interface {

public:

    bmclientInterface(_SharedPtr<Shell> owner);

    void init();
    void run();
    void handleKeys(int input);
    void draw(){};

protected:

    void resizeComponents(){}

private:

    _SharedPtr<GenericWindow> m_window;

};


