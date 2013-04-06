#pragma once


#ifdef LINUX


namespace ca
{


    class file;


} // namespace ca



typedef ::ca::file * HFILE;


#endif


namespace zip
{

   class File;

   class CLASS_DECL_ca InFile :
      public ::ca::file
   {
   public:

      enum e_mode
      {
         mode_undefined,
         mode_zip,
         mode_unzip,
      };

      unz_file_info                       m_fi;
      bool                                m_bCloseOnDelete;
      string                              m_strFileName;
      uint64_t                            m_iPosition;
      stringa                             m_straPath;
      spa(File)                           m_filea;
      spa(InFile)                         m_izfilea;
      stringa                             m_straPrefix;
      e_mode                              m_emode;
      string                              m_strZipFile;


      InFile(sp(::ca::application) papp);
      virtual ~InFile();



      operator HFILE() const;

      virtual file_position get_position() const;

      virtual bool zip_open(const char *,UINT);
      virtual bool zip_open(File * pzfile, const char * lpcszFileName);

      virtual bool unzip_open(::ca::file * pfile);
      virtual bool unzip_open(const char *,UINT);
      virtual bool unzip_open(File * pzfile, const char * lpcszFileName);

      virtual bool locate(const char * pszFileName);

      virtual void add_file(const char * pszDir, const char * pszRelative);

      bool dump(::ca::file * pfile);

      virtual ::ca::file * Duplicate() const;

      File * get_zip_file();
      const File * get_zip_file() const;

      virtual file_position seek(file_offset lOff, ::ca::e_seek  nFrom);
      virtual void set_length(file_size dwNewLen);
      virtual file_size get_length() const;

      virtual ::primitive::memory_size read(void * lpBuf, ::primitive::memory_size nCount);
      virtual void write(const void * lpBuf, ::primitive::memory_size nCount);

      virtual void LockRange(file_position dwPos, file_size dwCount);
      virtual void UnlockRange(file_position dwPos, file_size dwCount);

      virtual void Abort();
      virtual void flush();
      virtual void close();

   // Implementation
   public:
      virtual bool IsOpened();
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
      enum BufferCommand { bufferRead, bufferWrite, bufferCommit, bufferCheck };
      virtual uint64_t GetBufferPtr(UINT nCommand, uint64_t nCount = 0, void ** ppBufStart = ::null(), void ** ppBufMax = ::null());


   private:
      using ::ca::file::open;

   };

} // namespace zip

