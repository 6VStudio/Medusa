// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaPreCompiled.h"
#include "TTFFont.h"
#include "Core/IO/FileSystem.h"
#include "Core/Memory/MemoryData.h"
#include "Core/Log/Log.h"
#include "Resource/Font/FontFactory.h"
#include "Resource/TextureAtlas/TextureAtlasRegion.h"
#include "Resource/Material/MaterialFactory.h"
#include "Resource/ResourceNames.h"
#include "Resource/Image/PVRImage.h"
#include "Resource/Image/DynamicAtlasRGBAImage.h"
#include "Resource/Material/IMaterial.h"
#include "Resource/Texture/ImageTexture.h"
#include "Resource/Texture/TextureFactory.h"
#include "Resource/TextureAtlas/TextureAtlas.h"
#include "Resource/TextureAtlas/TextureAtlasPage.h"


MEDUSA_BEGIN;

TTFFont::TTFFont(const FontId& fontId) :IFont(fontId)
{
	mFace = nullptr;
}

TTFFont::TTFFont(const FontId& fontId, const MemoryByteData& data) : IFont(fontId), mFontData(data)
{
	mFace = nullptr;

	mInitialImageSize = 1024;
	mMaxImageSize = Render::Instance().GetInteger(GraphicsIntegerName::MaxTextureSize);

}


TTFFont::~TTFFont(void)
{
	FT_Done_Face(mFace);
}

bool TTFFont::InitializeLibrary()
{
	if (mLibrary == nullptr)
	{
		FT_Error err = FT_Init_FreeType(&mLibrary);
		if (err)
		{
			Log::Error("Error init freetype");
			return false;
		}
	}
	return true;
}

bool TTFFont::UninitializeLibrary()
{
	if (mLibrary != nullptr)
	{
		FT_Done_FreeType(mLibrary);
		mLibrary = nullptr;
	}
	return true;
}

TTFFont* TTFFont::CreateFromFile(const FontId& fontId)
{
	auto data = FileSystem::Instance().ReadAllData(fontId);
	TTFFont* font = new TTFFont(fontId, data);
	if (!font->Initialize())
	{
		SAFE_DELETE(font);
	}

	return font;

}


TTFFont* TTFFont::CreateFromData(const FontId& fontId, const MemoryByteData& data)
{
	TTFFont* font = new TTFFont(fontId, data);
	if (!font->Initialize())
	{
		SAFE_DELETE(font);
	}

	return font;
}


bool TTFFont::Initialize()
{
	RETURN_FALSE_IF_FALSE(InitializeLibrary());

	FT_Error err;
	if (!mFontData.IsNull())
	{
		err = FT_New_Memory_Face(mLibrary, mFontData.Data(), (FT_Long)mFontData.ByteSize(), 0, &mFace);
		if (err)
		{
			Log::Error("Error face");
			return false;
		}
	}
	else
	{
		Log::Error("Error face");
		return false;
	}


	//init some face metrics
	mFamilyName = mFace->family_name;
	mStyleName = mFace->style_name;
	mAscender = FontUnitToPixelSize(mFace->ascender);
	mDescender = FontUnitToPixelSize(mFace->descender);
	mTotalCharCount = (uint)mFace->num_glyphs;
	mFlags.SetOrRemoveIf(FontFlags::HasHorizontal, FT_HAS_HORIZONTAL(mFace) != 0);
	mFlags.SetOrRemoveIf(FontFlags::HasVertical, FT_HAS_VERTICAL(mFace) != 0);
	mFlags.SetOrRemoveIf(FontFlags::HasKerning, FT_HAS_KERNING(mFace) != 0);
	mFlags.SetOrRemoveIf(FontFlags::IsScalable, FT_IS_SCALABLE(mFace) != 0);
	mFlags.SetOrRemoveIf(FontFlags::IsItalic, (mFace->style_flags&FT_STYLE_FLAG_ITALIC) != 0);
	mFlags.SetOrRemoveIf(FontFlags::IsBold, (mFace->style_flags&FT_STYLE_FLAG_BOLD) != 0);
	mLineHeight = FontUnitToPixelSize(mFace->height);
	mMaxAdvance.Width = (uint)FontUnitToPixelSize(mFace->max_advance_width);
	mMaxAdvance.Height = (uint)FontUnitToPixelSize(mFace->max_advance_height);
	mUnderlinePosition = FontUnitToPixelSize(mFace->underline_position);
	if (mUnderlinePosition > -2)
	{
		mUnderlinePosition = -2;
	}

	mUnderlineTickness = FontUnitToPixelSize(mFace->underline_thickness);
	if (mUnderlineTickness < 1)
	{
		mUnderlineTickness = 1;
	}


	err = FT_Select_Charmap(mFace, FT_ENCODING_UNICODE);
	if (err)
	{
		Log::Error("Cannot select unicode charmap");
		return false;
	}

	OnUpdateFontId();

	mSpaceFontChar = OnLoadChar(' ');
	return true;
}

intp TTFFont::FontUnitToPixelSize(intp fontUnitSize) const
{
	return (int)mFontId.Size()*fontUnitSize / (int)mFace->units_per_EM;
}

intp TTFFont::FixedPointToPixelSize(intp val) const
{
	return val / 64;

}
bool TTFFont::OnUpdateFontId()
{
	/*The character widths and heights are specified in 1/64th of points. A point is a physical distance,
	equaling 1/72th of an inch. Normally, it is not equivalent to a pixel.*/
	/*character size is set to 16pt for a 300 * 300dpi*/

	/*err=FT_Set_Char_Size(mFace,0,16*64,300,300);
	if (err)
	{
	Log::LogError("Error char size");
	}*/

	FT_Error err = FT_Set_Pixel_Sizes(mFace, mFontId.Size(), mFontId.Size());
	if (err)
	{
		Log::Error("Error pixel size");
		return false;
	}

	//int maxPixelsX = ::FT_MulFix((mFace->bbox.xMax - mFace->bbox.xMin), mFace->size->metrics.x_scale);
	//int maxPixelsY = ::FT_MulFix((mFace->bbox.yMax - mFace->bbox.yMin), mFace->size->metrics.y_scale);
	

	const Matrix2& matrix = mFontId.Matrix();
	//Coefficients of the matrix are otherwise in 16.16 fixed-point units.
	if (matrix != Matrix2::Identity)
	{
		FT_Matrix    ftMatrix;
		ftMatrix.xx = (FT_Fixed)(matrix.A * 0x10000L);
		ftMatrix.xy = (FT_Fixed)(matrix.B * 0x10000L);
		ftMatrix.yx = (FT_Fixed)(matrix.C * 0x10000L);
		ftMatrix.yy = (FT_Fixed)(matrix.D * 0x10000L);
		/* the ftTranslate position in 26.6 cartesian space coordinates */
		FT_Vector ftTranslate;
		ftTranslate.x = (FT_Pos)matrix.X * 64;
		ftTranslate.y = (FT_Pos)matrix.Y * 64;
		FT_Set_Transform(mFace, &ftMatrix, &ftTranslate);
	}

	//update load flags
	mGlyphLoadFlags = 0;
	if (mFontId.HasOutline())
	{
		mGlyphLoadFlags |= FT_LOAD_NO_BITMAP;
	}
	else
	{
		if (mFontId.Depth() == FontImageDepth::MonoChrome)
		{
			mGlyphLoadFlags |= FT_LOAD_MONOCHROME;
		}
		else
		{
			mGlyphLoadFlags |= FT_LOAD_RENDER;
		}
	}

	if (mFontId.HasHinting())
	{
		mGlyphLoadFlags |= FT_LOAD_FORCE_AUTOHINT;
	}
	else
	{
		mGlyphLoadFlags |= FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
	}

	if (mFontId.HasLCDFiltering())
	{
		if (mFontId.LCDFilterType() == FontLCDFilterType::Light)
		{
			FT_Library_SetLcdFilter(mLibrary, FT_LCD_FILTER_LIGHT);
			FT_Library_SetLcdFilterWeights(mLibrary, (byte*)FontId::LightLCDFilter);
		}
		else
		{
			FT_Library_SetLcdFilter(mLibrary, FT_LCD_FILTER_DEFAULT);
			FT_Library_SetLcdFilterWeights(mLibrary, (byte*)FontId::DefaultLCDFilter);
		}

		mGlyphLoadFlags |= FT_LOAD_TARGET_LCD;
	}
	else
	{
		FT_Library_SetLcdFilter(mLibrary, FT_LCD_FILTER_NONE);
	}

	return true;

}
const FontChar* TTFFont::OnLoadChar(wchar_t c)
{
	FT_UInt glyphIndex = FT_Get_Char_Index(mFace, c);
	FT_Error err = FT_Load_Glyph(mFace, glyphIndex, mGlyphLoadFlags);
	if (err)
	{
		Log::Error("Error FT_Load_Glyph");
		return nullptr;
	}

	std::unique_ptr<FontChar> fontChar(new FontChar(c));
	/*    metrics           :: The metrics of the last loaded glyph in the   */
	/*                         slot.  The returned values depend on the last */
	/*                         load flags (see the @FT_Load_Glyph API        */
	/*                         function) and can be expressed either in 26.6 */
	/*                         fractional pixels or font units.              */
	/*                                                                       */
	/*                         Note that even when the glyph image is        */
	/*                         transformed, the metrics are not.             */

	FT_BitmapGlyph bitmapGlyph = nullptr;
	FT_GlyphSlot slot = mFace->glyph;


	FT_Bitmap bitmap;

	if (!mFontId.HasOutline())
	{
		bitmap = slot->bitmap;
		fontChar->HBearing.X = slot->bitmap_left;
		fontChar->HBearing.Y = slot->bitmap_top;
	}
	else
	{
		FT_Stroker stroker;
		err = FT_Stroker_New(mLibrary, &stroker);
		if (err)
		{
			Log::Error("Error FT_Stroker_New");
			FT_Stroker_Done(stroker);
			return nullptr;
		}

		FT_Stroker_Set(stroker, (int)(mFontId.OutlineThickness() * 64), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
		err = FT_Get_Glyph(slot, (FT_Glyph*)&bitmapGlyph);
		if (err)
		{
			Log::Error("Error FT_Get_Glyph");
			FT_Stroker_Done(stroker);
			return nullptr;
		}

		if (mFontId.OutlineType() == FontOutlineType::Line)
		{
			err = FT_Glyph_Stroke((FT_Glyph*)&bitmapGlyph, stroker, 1);

		}
		else if (mFontId.OutlineType() == FontOutlineType::Inner)
		{
			err = FT_Glyph_StrokeBorder((FT_Glyph*)&bitmapGlyph, stroker, 1, 1);

		}
		else if (mFontId.OutlineType() == FontOutlineType::Outer)
		{
			err = FT_Glyph_StrokeBorder((FT_Glyph*)&bitmapGlyph, stroker, 0, 1);
		}

		if (err)
		{
			Log::Error("Error FT_Glyph_Stroke");
			FT_Stroker_Done(stroker);
			return nullptr;
		}

		if (mFontId.Depth() == FontImageDepth::MonoChrome)
		{
			err = FT_Glyph_To_Bitmap((FT_Glyph*)&bitmapGlyph, FT_RENDER_MODE_MONO, 0, 1);
		}
		else
		{
			err = FT_Glyph_To_Bitmap((FT_Glyph*)&bitmapGlyph, FT_RENDER_MODE_LCD, 0, 1);
		}

		if (err)
		{
			Log::Error("Error FT_Glyph_To_Bitmap");
			FT_Stroker_Done(stroker);
			return nullptr;
		}

		FT_Stroker_Done(stroker);

		bitmap = bitmapGlyph->bitmap;
		fontChar->HBearing.X = bitmapGlyph->left;
		fontChar->HBearing.Y = bitmapGlyph->top;
	}

	if (bitmap.width != 0 && bitmap.rows != 0)	//empty char
	{
		// We want each glyph to be separated by at least one black pixel,but font factory already has spacing 1
		uint bitmapDepth = bitmap.pixel_mode != FT_PIXEL_MODE_BGRA ? 1 : 3;
		Size2U gylphSize(bitmap.width / bitmapDepth + 1, bitmap.rows + 1);
		auto* region = AddGlyphImage(c,mFontId.Depth(), gylphSize, bitmap.pitch, MemoryByteData::FromStatic(bitmap.buffer, 0), (FontImageDepth)bitmapDepth);

		if (region == nullptr)
		{
			if (bitmapGlyph != nullptr)
			{
				FT_Done_Glyph((FT_Glyph)bitmapGlyph);
			}

			return nullptr;
		}
		fontChar->SetRegion(region);
	}

	if (bitmapGlyph != nullptr)
	{
		FT_Done_Glyph((FT_Glyph)bitmapGlyph);
	}

	// Discard hinting to get advance
	FT_Load_Glyph(mFace, glyphIndex, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
	slot = mFace->glyph;
	fontChar->HAdvance = (ushort)FixedPointToPixelSize(slot->advance.x);
	fontChar->VAdvance = (ushort)FixedPointToPixelSize(slot->advance.y);

	mChars.Add(fontChar->Id, fontChar.get());

	return fontChar.release();
}

const FontKerning* TTFFont::OnLoadKerning(int prev, int next)
{
	FT_UInt leftGlyph = FT_Get_Char_Index(mFace, prev);
	FT_UInt rightGlyph = FT_Get_Char_Index(mFace, next);
	FT_Vector outKerning;
	FT_Get_Kerning(mFace, leftGlyph, rightGlyph, FT_KERNING_DEFAULT, &outKerning);
	if (outKerning.x != 0)
	{
		outKerning.x = (FT_Pos)FixedPointToPixelSize(outKerning.x);
		FontKerning* kerning = new FontKerning(prev, next, (int)outKerning.x);
		uint64 id = ((uint64)prev << 32) | ((uint64)next);
		mKernings.Add(id, kerning);
		return kerning;
	}

	return nullptr;
}

uint TTFFont::Preload(const WStringRef& str)
{
	RETURN_ZERO_IF_EMPTY(str);
	uint count = 0;
	size_t length = str.Length();
	FOR_EACH_SIZE(i, length)
	{
		wchar_t c = str[i];
		if (OnLoadChar(c) != nullptr)
		{
			++count;
		}
	}

	return count;
}


TextureAtlasRegion* TTFFont::AddGlyphImage(wchar_t c,FontImageDepth destDepth, const Size2U& size, int pitch, const MemoryByteData& imageData, FontImageDepth srcDepth)
{
	RETURN_NULL_IF(size > mMaxImageSize);

	GraphicsInternalFormat destInternalFormat = GraphicsInternalFormat::RGBA;
	GraphicsPixelFormat destPixelFormat = GraphicsPixelFormat::RGBA;
	GraphicsPixelDataType destDataType;

	switch (destDepth)
	{
	case FontImageDepth::RGBA:
		destInternalFormat = GraphicsInternalFormat::RGBA;
		destPixelFormat = GraphicsPixelFormat::RGBA;
		destDataType = GraphicsPixelDataType::Byte;
		break;
	case FontImageDepth::RGB:
		destInternalFormat = GraphicsInternalFormat::RGB;
		destPixelFormat = GraphicsPixelFormat::RGB;
		destDataType = GraphicsPixelDataType::Byte;
		break;
	case FontImageDepth::MonoChrome:
		destInternalFormat = GraphicsInternalFormat::Alpha;
		destPixelFormat = GraphicsPixelFormat::Alpha;
		destDataType = GraphicsPixelDataType::Byte;
		break;
	default:
		break;
	}

	//GraphicsInternalFormat srcInternalFormat;
	GraphicsPixelFormat srcPixelFormat = GraphicsPixelFormat::RGBA;
	GraphicsPixelDataType srcDataType;
	switch (srcDepth)
	{
	case FontImageDepth::RGBA:
		//srcInternalFormat = GraphicsInternalFormat::RGBA;
		srcPixelFormat = GraphicsPixelFormat::RGBA;
		srcDataType = GraphicsPixelDataType::Byte;
		break;
	case FontImageDepth::RGB:
		//srcInternalFormat = GraphicsInternalFormat::RGB;
		srcPixelFormat = GraphicsPixelFormat::RGB;
		srcDataType = GraphicsPixelDataType::Byte;
		break;
	case FontImageDepth::MonoChrome:
		//srcInternalFormat = GraphicsInternalFormat::Alpha;
		srcPixelFormat = GraphicsPixelFormat::Alpha;
		srcDataType = GraphicsPixelDataType::Byte;

		break;
	default:
		break;
	}

	Rect2U outRect=Rect2U::Zero;

	auto& pages= mAtlas->Pages();
	for (auto* page:pages)
	{
		DynamicAtlasRGBAImage* image = (DynamicAtlasRGBAImage*)page->GetTexture()->Image();
		if (image->AddImageRect(size, pitch, imageData, srcPixelFormat, srcDataType, outRect, true, GraphicsPixelConvertMode::Alpha))
		{
			page->SetPageSize(image->Size());

			TextureAtlasRegion* region = new TextureAtlasRegion();
			region->SetId(c);
			region->SetTextureRect(outRect);
			page->AddRegion(region);

			return region;
		}
	}


	//at this time all current material is full, add a new image
	DynamicAtlasRGBAImage* image = new DynamicAtlasRGBAImage("TTFFontImage", mInitialImageSize, mMaxImageSize, destInternalFormat, destPixelFormat, false);
	ImageTexture* texture = new ImageTexture(image->GetFileId(), image, ShaderSamplerNames::Texture, GraphicsTextureUnits::Texture0);
	texture->ResetDefaultParameters();

	TextureAtlasPage* page = new TextureAtlasPage(mAtlas->PageCount());
	page->SetTexture(texture);
	mAtlas->AddPage(page);

	if (image->AddImageRect(size, pitch, imageData, srcPixelFormat, srcDataType, outRect,  true, GraphicsPixelConvertMode::Alpha))
	{
		page->SetPageSize(image->Size());

		TextureAtlasRegion* region = new TextureAtlasRegion();
		region->SetId(c);
		region->SetTextureRect(outRect);
		page->AddRegion(region);

		return region;
	}
	return nullptr;
}


FT_Library TTFFont::mLibrary = nullptr;


MEDUSA_END;

