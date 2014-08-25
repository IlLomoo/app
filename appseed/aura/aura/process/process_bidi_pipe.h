#pragma once


namespace process
{


   class CLASS_DECL_AURA pipe:
      virtual public ::object
   {
   public:


      char *               m_pchBuf;
      string               m_strRead;

      bool                 m_bInherit;
      bool                 m_bBlock;


      pipe(sp(::aura::application) papp);
      virtual ~pipe();


      virtual bool create(bool bBlock = true,bool bInherit = false);


      virtual bool not_inherit_read();
      virtual bool not_inherit_write();

      virtual bool write(const char * psz);
      virtual string read();
      virtual string one_pass_read();


#ifdef WINDOWS

      void readex();

#endif



   };


   class CLASS_DECL_AURA bidi_pipe:
      virtual public ::object
   {
   public:


      sp(pipe)    m_sppipeIn;
      sp(pipe)    m_sppipeOut;


      bidi_pipe(sp(::aura::application) papp);
      virtual ~bidi_pipe();

      virtual bool create(bool bBlock = true,bool bInherit = false);


   };


} // namespace process


































