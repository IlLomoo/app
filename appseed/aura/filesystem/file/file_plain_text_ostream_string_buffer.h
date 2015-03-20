#pragma once


namespace file
{


   class CLASS_DECL_AURA plain_text_ostream_string_buffer :
      virtual public plain_text_ostream
   {
   public:


      string_buffer_sp        m_spstringbuffer;


      plain_text_ostream_string_buffer(::aura::application * papp = NULL);
      virtual ~plain_text_ostream_string_buffer();

      string str() const;

      operator const char *() const;

   };


} // namespace file

