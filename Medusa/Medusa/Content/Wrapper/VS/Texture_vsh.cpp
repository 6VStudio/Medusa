//This file was created by FileWrapper 1.0.0.0
//DO NOT EDIT
#include "MedusaPreCompiled.h"
#include "Core/IO/MemoryFileAutoRegister.h"
#pragma region Texture.vsh
MEDUSA_CONTENT_BEGIN;
const static char _Texture_vsh[] = "\
#ifndef GL_ES \n\
#define lowp\n\
#define mediump\n\
#define highp\n\
#endif\n\
\n\
attribute highp vec3	aVertex;\n\
attribute lowp vec4	aColor;\n\
attribute mediump vec2	aTexCoord;\n\
uniform highp mat4	uWorldViewProjectMatrix;\n\
varying mediump vec2	vTexCoord;\n\
varying lowp vec4 vColor;\n\
void main(void)\n\
{\n\
	gl_Position = uWorldViewProjectMatrix * vec4(aVertex, 1.0);\n\
	vTexCoord = aTexCoord;\n\
	vColor=aColor;\n\
}\n\
";
//const static MemoryFileAutoRegister mRegister_Texture_vsh(FileIdRef("Texture.vsh"),_Texture_vsh);
static void Register_Texture_vsh(){MemoryFileAutoRegister::Register(FileIdRef("Texture.vsh"),_Texture_vsh);}
MEDUSA_CONTENT_END;
#pragma endregion Texture.vsh

