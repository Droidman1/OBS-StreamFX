#include "obs-property.hpp"
#include "obs-properties.hpp"

#include "warning-disable.hpp"
#include <cassert>
#include "warning-enable.hpp"

::streamfx::obs::properties streamfx::obs::group_property::content()
{
#ifdef DEBUG
	assert(_self != nullptr);
#endif
	return obs_property_group_content(_self);
}
