#pragma once
//
//  tr1_wrapper.h
//  bmfec-client
//
//  Created by Jonathan Rumion on 8/19/14.
//  Copyright (c) 2014 TAP. All rights reserved.
//


//  C++ Defines.

#ifdef __linux__
#define _CINTTYPES <tr1/cinttypes>
#define _MEMORY <tr1/memory>
//#include <tr1/shared_ptr.h>
#endif
#ifdef __APPLE__
#define _CINTTYPES <inttypes>
#define _MEMORY <memory>
#endif

#include <memory>

#ifdef __linux__
#define _SharedPtr std::shared_ptr
#define _WeakPtr boost::weak_ptr
#define _UniquePtr boost::unique_ptr
#endif
#ifdef __APPLE__
#define _SharedPtr std::shared_ptr
#define _WeakPtr std::weak_ptr
#define _UniquePtr std::unique_ptr
#endif




