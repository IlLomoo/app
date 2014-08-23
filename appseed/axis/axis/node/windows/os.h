#pragma once

//#include <shlobj.h>
//#include <Security.h>
//#include <shlobj.h>

//#include "windows_exception.h"





string get_error_message(DWORD dwError);




namespace windows
{
   

   class interaction_impl;


} // namespace windows




#include "win1.h"
#include "windows_implementation.h"
#include "windows_message_queue.h"
#include "windows_factory_exchange.h"
#include "windows_window_draw.h"
#include "windows_interaction_impl.h"

CLASS_DECL_AXIS WNDPROC __get_window_procedure();
#define __window_procedure (*__get_window_procedure())

#define NODE_WINDOW(pwnd) ((sp(::windows::interaction_impl))(pwnd))



CLASS_DECL_AXIS void __trace_message(const char * lpszPrefix, signal_details * pobj);
CLASS_DECL_AXIS void __trace_message(const char * lpszPrefix, LPMSG lpmsg);



#include "windows_print_job.h"



void CLASS_DECL_AXIS __cdecl _ca2_purecall();
void CLASS_DECL_AXIS __cdecl _null_se_translator(uint32_t uiCode, EXCEPTION_POINTERS * ppointers);
bool CLASS_DECL_AXIS __windows_init();
int32_t CLASS_DECL_AXIS __windows_main(sp(::aura::system) psystem, ::windows::main_init_data * pmaininitdata);





#include "windows_copydesk.h"


// Sanity checks for ATOMs
CLASS_DECL_AXIS bool __is_valid_atom(ATOM nAtom);
CLASS_DECL_AXIS bool __is_valid_atom(const char * psz);



#define __set_dialog_control_id(oswindow, nID)     SetWindowLong(oswindow, GWL_ID, nID)
#define __set_dialog_control_id_(oswindow, nID)     oswindow->SetWindowLong(GWL_ID, nID)
#define __get_dialog_control_id(oswindow)         ((UINT)(WORD)::GetDlgCtrlID(oswindow))
#define __get_dialog_control_id_(oswindow)         ((UINT)(WORD)oswindow->GetDlgCtrlId())
