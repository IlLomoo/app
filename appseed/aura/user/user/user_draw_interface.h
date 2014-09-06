#pragma once


namespace user
{


   class CLASS_DECL_AURA draw_interface
   {
   public:


      virtual void _001OnDraw(::draw2d::graphics * pdc);

      virtual void _001DeferPaintLayeredWindowBackground(::draw2d::graphics * pdc) = 0;

      virtual void _001OnDeferPaintLayeredWindowBackground(::draw2d::graphics * pdc) = 0;

      virtual void GetWindowRect(LPRECT lprect) = 0;
      virtual void GetWindowRect(__rect64 * lprect) = 0;
      virtual rect GetWindowRect() = 0;
      virtual rect64 GetWindowRect64() = 0;


   };


} // namespace user


