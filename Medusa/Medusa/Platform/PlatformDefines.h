// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
#include "CorePlatform/CorePlatformDefines.h"


//Features
#ifndef MEDUSA_DISABLE_AL
#define MEDUSA_AL
#endif

#ifndef MEDUSA_DISABLE_FREETYPE
#define MEDUSA_FREETYPE
#endif

#ifndef MEDUSA_DISABLE_JPEG
#define MEDUSA_JPEG
#endif

#ifndef MEDUSA_DISABLE_PNG
#define MEDUSA_PNG
#endif

#ifndef MEDUSA_DISABLE_OGG
#define MEDUSA_OGG
#endif

#ifndef MEDUSA_DISABLE_BOX2D
#define MEDUSA_BOX2D
#endif


#ifndef MEDUSA_DISABLE_GL
#define MEDUSA_GL
#endif


#define MEDUSA_RENDER_ASSERT_ENABLED
#define MEDUSA_RENDER_STATE_CACHE_ENABLED
#define MEDUSA_PRE_GENERATED_INDEX_ENABLED

#define MEDUSA_AUDIO_ASSERT_ENABLED
#define MEDUSA_AUDIO_STATE_CACHE_ENABLED
//#define MEDUSA_AUDIO_ASSERT_ENABLED

#ifdef MEDUSA_WINDOWS
#define MEDUSA_AUDIO_DEFAULT_MAX_SOURCE_COUNT 32
#define MEDUSA_GLES
//#define MEDUSA_GLEW

#endif

#ifdef MEDUSA_IOS
#define MEDUSA_AUDIO_DEFAULT_MAX_SOURCE_COUNT 32
#define MEDUSA_GLES
#endif

#ifdef MEDUSA_ANDROID
#define MEDUSA_AUDIO_DEFAULT_MAX_SOURCE_COUNT 32
#define MEDUSA_GLES
#endif

/************************************************************************/
//use this to identifier with open gl
#define MEDUSA_IS_OPENGL 1
#define MEDUSA_IS_DIRECTX 0

/************************************************************************/


#define MEDUSA_FPS	60.f
#define MEDUSA_FRAME_INTERVAL	1.f/60.f

#define MEDUSA_PREFIX(x) MACRO_TO_STRING(MACRO_CONCAT(Medusa_,x))


#define MEDUSA_SCREEN_WIDTH 1600U
#define MEDUSA_SCREEN_HEIGHT 900U