#pragma once


namespace datetime
{


   class CLASS_DECL_CORE element :
      virtual public ::element
   {
   public:

      element();
      ~element();

      token *     m_ptoken;
      element *   m_pparent;
      element *   m_pelement1;
      element *   m_pelement2;
      element *   m_pelement3;


      value get_value(sp(::base::application) pbaseapp, ::user::str_context * pcontext, int32_t & iPath, int32_t & iPathCount) const;
      string get_expression(sp(::base::application) pbaseapp, ::user::str_context * pcontext, int32_t & iPath, int32_t & iPathCount) const;
         



        
   };


} // namespace calculator



