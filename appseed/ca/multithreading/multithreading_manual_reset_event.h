//*****************************************************************************
//
//      Class:          manual_reset_event
//      Author:         Kenny Kerr
//      Date created:   10 April 2004
//      Description:    Notifies one or more waiting threads that an event has 
//                      occurred.
//
//*****************************************************************************


#pragma once


class CLASS_DECL_ca manual_reset_event :
   public event
{
public:


   manual_reset_event(::ca::applicationsp papp, bool bInitiallyOwn = false);

   
};


