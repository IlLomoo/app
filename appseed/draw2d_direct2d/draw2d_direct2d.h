#pragma once


#include "ca2.h"


#ifdef _CA2_DRAW2D_DIRECT2D_LIBRARY
    #define CLASS_DECL_DRAW2D_DIRECT2D  _declspec(dllexport)
#else
    #define CLASS_DECL_DRAW2D_DIRECT2D  _declspec(dllimport)
#endif


namespace draw2d_direct2d
{


   class graphics;

   inline void throw_if_failed(HRESULT hr)
   {

      if(FAILED(hr))
         throw hresult_exception(::ca2::get_thread_app(), hr);
      //if(FAILED(hr))
      // throw hr;

   }


} // namespace draw2d_direct2d


#include "draw2d_direct2d_factory_exchange.h"


#include "draw2d_direct2d_object.h"
#include "draw2d_direct2d_pen.h"
#include "draw2d_direct2d_bitmap.h"
#include "draw2d_direct2d_brush.h"
#include "draw2d_direct2d_font.h"
#include "draw2d_direct2d_palette.h"
#include "draw2d_direct2d_region.h"
#include "draw2d_direct2d_dib.h"
#include "draw2d_direct2d_path.h"
#include "draw2d_direct2d_printer.h"


#include "draw2d_direct2d_graphics.h"


#pragma comment(lib, "Msimg32.lib") 


