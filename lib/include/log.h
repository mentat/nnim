// --*-c++-*--
/////////////////////////////////////////////////////////////////////////////
// Name:        log.h
// Purpose:     a stab at a cross-platform logging system
// Author:      Jesse Lovelace
// Modified by:
// Created:
// RCS-ID:      $Id: log.h,v 1.1 2002/06/06 17:21:53 thementat Exp $
// Copyright:   (c) Jesse Lovelace
// Licence:     LGPL licence
/////////////////////////////////////////////////////////////////////////////

#ifndef KIM_LOG_H
#define KIM_LOG_H

#include <string>
#include <iostream>
// I decided that i'm an idiot an changed this class to macros
using namespace std;

//#ifndef LOGGER
//#define LOGGER( text ) cout << text << endl
//#endif


// - Just so I dont have to go change all the files.
//#ifndef kLog
//#define kLog( text ) cout << text << endl
//#endif

ostream &debug();
ostream &debug(int priority);
ostream &critical();
ostream &error();


#endif
