#pragma once

#include <yasli/Archive.h>
#include <Serialization/Serializer.h>

namespace Serialization{

typedef yasli::Archive IArchive;
typedef yasli::Serializer SStruct;
typedef yasli::Context SContext;
typedef yasli::Context SContextLink;
typedef yasli::TypeID TypeID;
typedef std::vector<SStruct> SStructs;

}
