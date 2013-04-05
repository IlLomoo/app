#pragma once


namespace dynamic_source
{

   class script;

   class CLASS_DECL_ca script_cache :
      virtual public ::ca::object
   {
   public:


      critical_section     m_cs;
      strsp(script)        m_map;
      sp(script_manager)   m_pmanager;


      script_cache(::ca::applicationsp papp);
      ~script_cache();

      script_instance * create_instance(const char * lpcszName);

      void set_all_out_of_date();

      void cache(script * pscript);

      script * get(const char * lpcszName);
      script * register_script(const char * lpcszName, script * pscript);


   };


} // namespace dynamic_source


