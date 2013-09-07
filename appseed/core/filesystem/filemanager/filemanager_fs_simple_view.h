#pragma once

namespace filemanager
{


   namespace fs
   {

      namespace simple
      {
      
         class tree_view;
         class list_view;

         class CLASS_DECL_ca2 view :
            virtual public ::user::split_view
         {
         public:
            tree_view * m_ptree;
            sp(list_view) m_plist;
            request_interface * m_prequestinterface;

            view(sp(base_application) papp);

            void install_message_handling(::message::dispatch * pinterface);

            void open_file(int64_t iFolder, const char * lpszFileName, const char * pszExt);
            void open_folder(int64_t iFolder);
            void start_music();
            void start_root();
            void CreateViews();

            void set_request_interface(request_interface * prequestinterface);

            virtual ~view();
         #ifdef DEBUG
            virtual void assert_valid() const;
            virtual void dump(dump_context & dumpcontext) const;
         #endif

            DECL_GEN_SIGNAL(on_request_response);

         };


      } // namespace simple


   } // namespace fs


} // namespace mplite
