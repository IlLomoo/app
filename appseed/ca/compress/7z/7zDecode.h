// 7zDecode.h
// from 7-zip on dawn of 13/01/2001 - Thursday

#pragma once

namespace n7z
{

   struct CBindInfoEx: public ::libcompress::coder_mixer::CBindInfo
   {
      base_array < ::libcompress::method_id > CoderMethodIDs;
      void remove_all()
      {
         CBindInfo::remove_all();
         CoderMethodIDs.remove_all();
      }
   };

   class CDecoder :
      virtual public ::ca::object
   {
      bool _bindInfoExPrevIsDefined;
      CBindInfoEx _bindInfoExPrev;

      bool _multiThread;
      ::libcompress::coder_mixer::CCoderMixer2MT *_mixerCoderMTSpec;
      ::libcompress::coder_mixer::CCoderMixer2 *_mixerCoderCommon;

      ::ca::smart_pointer < ::libcompress::coder2_interface > _mixerCoder;
      base_array < ::ca::smart_pointer < ::ca::ca > > _decoders;
      // ::collection::smart_pointer_array<::ca::smart_pointer<::libcompress::coder2_interface> > _decoders2;
   public:
      CDecoder(::ca::application * papp,  bool multiThread);
      virtual ~CDecoder();
      HRESULT Decode(
         ::libcompress::codecs_info_interface * codecsInfo, const base_array < ::libcompress::codec_info_ex > *externalCodecs,
         ::ca::byte_input_stream *inStream,
         file_position startPos,
         const file_size * packSizes,
         const CFolder &folder,
         ::ca::writer *outStream,
         ::libcompress::progress_info_interface *compressProgress,
         ::crypto::get_text_password_interface *getTextPasswordSpec, bool &passwordIsDefined,
         bool mtMode, uint32_t numThreads
         );
   };

} // namespace n7z

