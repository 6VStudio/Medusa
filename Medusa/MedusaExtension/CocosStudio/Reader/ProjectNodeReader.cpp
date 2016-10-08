#include "MedusaExtensionPreCompiled.h"
#include "ProjectNodeReader.h"
#include "Core/String/StringRef.h"
#include "Core/IO/FileId.h"
#include "Node/Sprite/Sprite.h"
#include "Node/NodeFactory.h"
#include "ReaderFactory.h"
#include "Core/IO/FileSystem.h"
#include "Node/Editor/INodeEditor.h"
#include "CocosStudio/CSParseBinary_generated.h"
#include "Core/IO/Path.h"

MEDUSA_COCOS_BEGIN;

INode* ProjectNodeReader::CreateNodeWithFlatBuffers(INodeEditor& editor, const flatbuffers::Table* nodeOptions, const StringRef& className, NodeCreateFlags flags /*= NodeCreateFlags::None*/)
{
	auto projectNodeOptions = (flatbuffers::ProjectNodeOptions*)nodeOptions;
	StringRef filePath = projectNodeOptions->fileName()->c_str();
	auto fileId = FileId::ParseFrom(filePath);
	RETURN_NULL_IF_FALSE(FileSystem::Instance().AssertExists(fileId));

	INode* node = NodeFactory::Instance().Create(fileId, IEventArg::Empty, flags);
	SetPropsWithFlatBuffers(node, (flatbuffers::Table*)(projectNodeOptions->nodeOptions()), flags);
	return node;
}

INode* ProjectNodeReader::CreateNodeWithJson(INodeEditor& editor, const rapidjson::Value& nodeTree, const StringRef& className /*= StringRef::Empty*/, NodeCreateFlags flags /*= NodeCreateFlags::None*/)
{
	const rapidjson::Value& fileDataNode = nodeTree["FileData"];
	StringRef filePath = fileDataNode.GetMember("Path", nullptr);
	auto fileId = FileId::ParseFrom(filePath);
	RETURN_NULL_IF_FALSE(FileSystem::Instance().AssertExists(fileId));

	INode* node = NodeFactory::Instance().Create(fileId, IEventArg::Empty, flags);
	SetPropsWithJson(node, nodeTree, flags);
	return node;
}

MEDUSA_COCOS_END;
