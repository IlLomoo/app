#include "framework.h"


namespace http
{


   memory_file::memory_file(::ca::applicationsp papp) :
      ca(papp),
      ::primitive::memory_file(papp),
      primitive::memory_container(papp)
   {
   }
      
   void memory_file::to_string(string & str)
   {
      ::primitive::memory_file::to_string(str);
   }


} // namespace http


