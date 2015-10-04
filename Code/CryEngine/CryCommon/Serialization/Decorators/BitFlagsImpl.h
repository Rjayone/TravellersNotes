#pragma once

#include <Serialization/Forward.h>
#include <Serialization/IArchive.h>
#include <Serialization/Decorators/BitFlags.h>

namespace Serialization {

inline void BitFlagsWrapper::Serialize(IArchive& ar)
{
	const yasli::EnumDescription& desc = *description;
	int count = desc.count();
	if (ar.isInput()) {
		int previousValue = *variable;
		for (int i = 0; i < count; ++i) {
			int flagValue = desc.valueByIndex(i);
			if (!(flagValue & visibleMask))
				continue;
			bool flag = (previousValue & flagValue) == flagValue;
			bool previousFlag = flag;
			ar(flag, desc.nameByIndex(i), desc.labelByIndex(i));
			if (flag != previousFlag)
			{
				if (flag)
					*variable |= flagValue;
				else
					*variable &= ~flagValue;
			}
		}
	}
	else {
		for (int i = 0; i < count; ++i) {
			int flagValue = desc.valueByIndex(i);
			if (!(flagValue & visibleMask))
				continue;
			bool flag = (*variable & flagValue) == flagValue;
			ar(flag, desc.nameByIndex(i), desc.labelByIndex(i));
		}
	}
}

}
