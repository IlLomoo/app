#pragma once


namespace spa
{

   class CLASS_DECL_c canvas
   {
   public:

      int      m_iMode;
      int      m_iModeCount;

      canvas();
      ~canvas();

      void on_paint(HDC hdc, LPCRECT lpcrect);


      int increment_mode();


   };


} // namespace spa