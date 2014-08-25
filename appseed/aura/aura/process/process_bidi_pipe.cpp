#include "framework.h"


namespace process
{






   bool pipe::not_inherit_read()
   {

      return true;

   }

   bool pipe::not_inherit_write()
   {

      return true;

   }


   bool pipe::write(const char * psz)
   {

      return true;

   }


   string pipe::read()
   {

      return "";

   }


   string pipe::one_pass_read()
   {

      return "";

   }


#ifdef WINDOWS

   void pipe::readex()
   {

   }

#endif


   cross_pipe::cross_pipe(sp(::aura::application) papp):
      element(papp),
      m_sppipeIn(allocer()),
      m_sppipeOut(allocer())
   {

   }


   bool cross_pipe::create(bool bBlock,bool bInherit)
   {

      if(!m_sppipeIn->create(bBlock,bInherit))
         return false;

      if(!m_sppipeIn->not_inherit_write())
         return false;

      if(!m_sppipeOut->create(bBlock,bInherit))
         return false;

      if(!m_sppipeOut->not_inherit_read())
         return false;

      return true;

   }


} // namespace core





































