#include "framework.h"


namespace ca
{


   output_file_stream::output_file_stream(sp(::ca::application) papp, const char * pszFileName, uint32_t dwFlags)
   {

      m_spfile = App(papp).file().get_file(pszFileName, dwFlags);

   }
      
   output_file_stream::~output_file_stream()
   {
   }



} // namespace ca


