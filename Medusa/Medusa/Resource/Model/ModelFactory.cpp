// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#include "MedusaPreCompiled.h"
#include "ModelFactory.h"
#include "Resource/Model/Basic/QuadModel.h"
#include "Resource/Model/Font/BaseFontModel.h"
#include "Resource/Model/Font/MultipleLineFontModel.h"
#include "Resource/Model/Font/SingleLineFontModel.h"

#include "Resource/Model/Scene/PODModel.h"
#include "Resource/Font/FontFactory.h"
#include "Resource/Material/MaterialFactory.h"
#include "Resource/ResourceNames.h"
#include "Core/IO/FileInfo.h"

MEDUSA_BEGIN;

ModelFactory::ModelFactory()
{

}

ModelFactory::~ModelFactory()
{
	
}

bool ModelFactory::Initialize()
{
	return true;
}

bool ModelFactory::Uninitialize()
{
	Clear();
	return true;
}


Share<QuadModel> ModelFactory::CreateQuad( const FileIdRef& fileId ,ResourceShareType shareType /*= ResourceShareType::Share*/)
{
	Share<QuadModel> model = nullptr;
	if (shareType != ResourceShareType::None)
	{
		model = Find(fileId).CastPtr<QuadModel>();
		RETURN_SELF_IF_NOT_NULL(model);
	}

	auto material= MaterialFactory::Instance().CreateSingleTexture(fileId);

	model=new QuadModel(fileId,material,Rect2I(Point2I::Zero,material->FirstTexture()->Size()));
	model->Initialize();
	Add(model, shareType);

	return model;
}

Share<SingleLineFontModel> ModelFactory::CreateSingleLineFontModel(const FontId& fontId,StringRef text,Alignment alignment/*=Alignment::LeftBottom*/,Size2U restrictSize/*=Size2U::Zero*/ )
{
	auto font=FontFactory::Instance().Create(fontId);
	Share<SingleLineFontModel> model=new SingleLineFontModel(font,alignment,restrictSize);
	model->Initialize();
	model->SetText(text);
	return model;

}

Share<MultipleLineFontModel> ModelFactory::CreateMultipleLineFontModel(const FontId& fontId,StringRef text,Alignment alignment/*=Alignment::LeftBottom*/,Size2U restrictSize/*=Size2U::Zero*/)
{
	auto font=FontFactory::Instance().Create(fontId);
	RETURN_NULL_IF_NULL(font);
	Share<MultipleLineFontModel> model=new MultipleLineFontModel(font,alignment,restrictSize);
	model->Initialize();
	model->SetText(text);
	return model;

}

Share<IModel> ModelFactory::Create( const FileIdRef& fileId,ModelLoadingOptions loadingOptions/*=ModelLoadingOptions::None*/ ,ResourceShareType shareType /*= ResourceShareType::Share*/)
{
	Share<IModel> model;
	if (shareType != ResourceShareType::None)
	{
		model = Find(fileId);
		RETURN_SELF_IF_NOT_NULL(model);
	}

	

	switch(FileInfo::ExtractType(fileId.Name))
	{
	case FileType::pod:
		{
			model=PODModel::CreateFromFile(fileId,loadingOptions);
			Add(model, shareType);
			return model;
		}
		break;
	default:
		break;
	}

	return nullptr;
}


MEDUSA_END;