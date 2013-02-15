#ifndef __GNUC__
#pragma once
#endif


#include <ImfAcesFile.h>
#include <ImfRgbaFile.h>
#include <ImfStandardAttributes.h>
#include <Iex.h>
#include <ImfAttribute.h>
#include "IlmThreadMutex.h"
#include "Iex.h"
#include <string.h>
#include <map>
#include <ImfB44Compressor.h>
#include <ImfHeader.h>
#include <ImfChannelList.h>
#include <ImfMisc.h>
#include <ImfCheckedArithmetic.h>
#include <ImathFun.h>
#include <ImathBox.h>
#include <Iex.h>
#include <ImfIO.h>
#include <ImfXdr.h>
#include <string.h>
#include <assert.h>
#include <algorithm>
#include <ImfBoxAttribute.h>
#include <ImfChannelList.h>
#include <Iex.h>
#include <ImfChannelListAttribute.h>
#include <ImfChromaticities.h>
#include <ImfChromaticitiesAttribute.h>
#include <ImfCompressionAttribute.h>
#include <ImfCompressor.h>
#include <ImfRleCompressor.h>
#include <ImfZipCompressor.h>
#include <ImfPizCompressor.h>
#include <ImfPxr24Compressor.h>
#include <ImfB44Compressor.h>
#include <ImfCheckedArithmetic.h>
#include <ImfConvert.h>
#include <limits.h>
#include <ImfCRgbaFile.h>
#include <ImfRgbaFile.h>
#include <ImfTiledRgbaFile.h>
#include <ImfIntAttribute.h>
#include <ImfFloatAttribute.h>
#include <ImfDoubleAttribute.h>
#include <ImfStringAttribute.h>
#include <ImfBoxAttribute.h>
#include <ImfVecAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfChannelList.h>
#include <ImfLut.h>
#include "half.h"
#include <string.h>
#include <ImfDoubleAttribute.h>
#include <ImfEnvmap.h>
#include "ImathFun.h"
#include <algorithm>
#include <math.h>
#include <ImfEnvmapAttribute.h>
#include <ImfFloatAttribute.h>
#include <ImfFrameBuffer.h>
#include "Iex.h"
#include <ImfFramesPerSecond.h>
#include "ImathFun.h"
#include <ImfHeader.h>
#include <ImfStdIO.h>
#include <ImfVersion.h>
#include <ImfCompressor.h>
#include <ImfMisc.h>
#include <ImfBoxAttribute.h>
#include <ImfChannelListAttribute.h>
#include <ImfChromaticitiesAttribute.h>
#include <ImfCompressionAttribute.h>
#include <ImfDoubleAttribute.h>
#include <ImfEnvmapAttribute.h>
#include <ImfFloatAttribute.h>
#include <ImfIntAttribute.h>
#include <ImfKeyCodeAttribute.h>
#include <ImfLineOrderAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfOpaqueAttribute.h>
#include <ImfPreviewImageAttribute.h>
#include <ImfRationalAttribute.h>
#include <ImfStringAttribute.h>
#include <ImfStringVectorAttribute.h>
#include <ImfTileDescriptionAttribute.h>
#include <ImfTimeCodeAttribute.h>
#include <ImfVecAttribute.h>
#include "IlmThreadMutex.h"
#include "Iex.h"
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <ImfHuf.h>
#include <ImfInt64.h>
#include <ImfAutoArray.h>
#include "Iex.h"
#include <string.h>
#include <assert.h>
#include <algorithm>
#include <ImfInputFile.h>
#include <ImfScanLineInputFile.h>
#include <ImfTiledInputFile.h>
#include <ImfChannelList.h>
#include <ImfMisc.h>
#include <ImfStdIO.h>
#include <ImfVersion.h>
#include "ImathFun.h"
#include "IlmThreadMutex.h"
#include "Iex.h"
#include "half.h"
#include <fstream>
#include <algorithm>
#include <ImfIntAttribute.h>
#include <ImfIO.h>
#include "Iex.h"
#include <ImfKeyCode.h>
#include "Iex.h"
#include <ImfKeyCodeAttribute.h>
#include <ImfLineOrderAttribute.h>
#include <ImfLut.h>
#include <math.h>
#include <assert.h>
#include <ImfMatrixAttribute.h>
#include <ImfMisc.h>
#include <ImfHeader.h>
#include <ImfCompressor.h>
#include <ImfChannelList.h>
#include <ImfXdr.h>
#include <ImathFun.h>
#include <Iex.h>
#include <ImfStdIO.h>
#include <ImfConvert.h>
#include <ImfMultiView.h>
#include <ImfOpaqueAttribute.h>
#include "Iex.h"
#include <string.h>
#include <ImfOutputFile.h>
#include <ImfInputFile.h>
#include <ImfChannelList.h>
#include <ImfMisc.h>
#include <ImfStdIO.h>
#include <ImfCompressor.h>
#include "ImathBox.h"
#include "ImathFun.h"
#include <ImfArray.h>
#include <ImfXdr.h>
#include <ImfPreviewImageAttribute.h>
#include "IlmThreadPool.h"
#include "IlmThreadSemaphore.h"
#include "IlmThreadMutex.h"
#include "Iex.h"
#include <string>
#include <vector>
#include <fstream>
#include <assert.h>
#include <ImfPizCompressor.h>
#include <ImfHeader.h>
#include <ImfChannelList.h>
#include <ImfHuf.h>
#include <ImfWav.h>
#include <ImfMisc.h>
#include <ImfCheckedArithmetic.h>
#include <ImathFun.h>
#include <ImathBox.h>
#include <Iex.h>
#include <ImfIO.h>
#include <ImfXdr.h>
#include <ImfAutoArray.h>
#include <string.h>
#include <assert.h>
#include <ImfPreviewImage.h>
#include <ImfCheckedArithmetic.h>
#include "Iex.h"
#include <ImfPreviewImageAttribute.h>
#include <ImfPxr24Compressor.h>
#include <ImfHeader.h>
#include <ImfChannelList.h>
#include <ImfMisc.h>
#include <ImfCheckedArithmetic.h>
#include <ImathFun.h>
#include <Iex.h>
#include <half.h>
#include <zlib/zlib.h>
#include <assert.h>
#include <algorithm>
#include <ImfRational.h>
#include <cmath>
#include <ImfRationalAttribute.h>
#include <ImfRgbaFile.h>
#include <ImfOutputFile.h>
#include <ImfInputFile.h>
#include <ImfChannelList.h>
#include <ImfRgbaYca.h>
#include <ImfStandardAttributes.h>
#include <ImathFun.h>
#include <IlmThreadMutex.h>
#include <Iex.h>
#include <string.h>
#include <algorithm>
#include <ImfRgbaYca.h>
#include <assert.h>
#include <algorithm>
#include <ImfRleCompressor.h>
#include <ImfCheckedArithmetic.h>
#include "Iex.h"
#include <ImfScanLineInputFile.h>
#include <ImfChannelList.h>
#include <ImfMisc.h>
#include <ImfStdIO.h>
#include <ImfCompressor.h>
#include "ImathBox.h"
#include "ImathFun.h"
#include <ImfXdr.h>
#include <ImfConvert.h>
#include <ImfThreading.h>
#include "IlmThreadPool.h"
#include "IlmThreadSemaphore.h"
#include "IlmThreadMutex.h"
#include "Iex.h"
#include <string>
#include <vector>
#include <assert.h>
#include <ImfStandardAttributes.h>
#include <ImfStdIO.h>
#include "Iex.h"
#include <errno.h>
#include <ImfStringAttribute.h>
#include <ImfStringVectorAttribute.h>
#include <ImfTestFile.h>
#include <ImfStdIO.h>
#include <ImfXdr.h>
#include <ImfVersion.h>
#include "IlmThreadPool.h"
#include <ImfTileDescriptionAttribute.h>
#include <ImfTiledInputFile.h>
#include <ImfTileDescriptionAttribute.h>
#include <ImfChannelList.h>
#include <ImfMisc.h>
#include <ImfTiledMisc.h>
#include <ImfStdIO.h>
#include <ImfCompressor.h>
#include "ImathBox.h"
#include <ImfXdr.h>
#include <ImfConvert.h>
#include <ImfVersion.h>
#include <ImfTileOffsets.h>
#include <ImfThreading.h>
#include "IlmThreadPool.h"
#include "IlmThreadSemaphore.h"
#include "IlmThreadMutex.h"
#include "ImathVec.h"
#include "Iex.h"
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <ImfTiledMisc.h>
#include "Iex.h"
#include <ImfMisc.h>
#include <ImfChannelList.h>
#include <ImfTiledOutputFile.h>
#include <ImfTiledInputFile.h>
#include <ImfInputFile.h>
#include <ImfTileDescriptionAttribute.h>
#include <ImfPreviewImageAttribute.h>
#include <ImfChannelList.h>
#include <ImfMisc.h>
#include <ImfTiledMisc.h>
#include <ImfStdIO.h>
#include <ImfCompressor.h>
#include "ImathBox.h"
#include <ImfArray.h>
#include <ImfXdr.h>
#include <ImfVersion.h>
#include <ImfTileOffsets.h>
#include <ImfThreading.h>
#include "IlmThreadPool.h"
#include "IlmThreadSemaphore.h"
#include "IlmThreadMutex.h"
#include "Iex.h"
#include <string>
#include <vector>
#include <fstream>
#include <assert.h>
#include <map>
#include <ImfTiledRgbaFile.h>
#include <ImfRgbaFile.h>
#include <ImfTiledOutputFile.h>
#include <ImfTiledInputFile.h>
#include <ImfChannelList.h>
#include <ImfTileDescriptionAttribute.h>
#include <ImfStandardAttributes.h>
#include <ImfRgbaYca.h>
#include <ImfArray.h>
#include "IlmThreadMutex.h"
#include "Iex.h"
#include <ImfTileOffsets.h>
#include <ImfXdr.h>
#include <ImfIO.h>
#include "Iex.h"
#include <ImfTimeCode.h>
#include "Iex.h"
#include <ImfTimeCodeAttribute.h>
#include <ImfVecAttribute.h>
#include <ImfVersion.h>
#include <ImfWav.h>
#include <ImfZipCompressor.h>
#include <ImfCheckedArithmetic.h>
#include "Iex.h"
#include <zlib/zlib.h>


using namespace std;
using namespace Imath;
using namespace Iex;
