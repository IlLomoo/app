//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


namespace user
{


   schema_simple_impl::schema_simple_impl(::aura::application * papp)  :
      object(papp)
   {

      m_etranslucency      = TranslucencyUndefined;

      m_pfont.alloc(allocer());

      m_pfont->create_pixel_font("Helvetica",16);

      m_pfont.alloc(allocer());

      m_pfont->create_pixel_font("Helvetica",16);

   }


   schema_simple_impl::~schema_simple_impl()
   {

   }


   bool schema_simple_impl::set_color(e_color ecolor,COLORREF cr)
   {

      m_mapColor[ecolor] = cr;

      return true;

   }


   bool schema_simple_impl::get_color(COLORREF & cr, e_color ecolor)
   {

      return m_mapColor.Lookup(ecolor,cr);

   }

   bool schema_simple_impl::get_font(::draw2d::font_sp & font)
   {

      if(m_pfont.is_null())
         return false;

      font = m_pfont;

      return true;

   }


   bool schema_simple_impl::get_translucency(ETranslucency & etranslucency)
   {

      if(m_etranslucency == TranslucencyUndefined)
         return false;

      etranslucency = m_etranslucency;

      return true;

   }


} // namespace user











