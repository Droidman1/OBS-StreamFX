
#pragma once
#include "common.hpp"
#include "plugin.hpp"

#include "warning-disable.hpp"
#include <memory>
#include <string>
#include <string_view>
#include "warning-enable.hpp"

namespace streamfx::obs {
	class properties;

	class property {
		protected:
		obs_property_t* _self;

		public:
		FORCE_INLINE ~property()
		{
		}

		/** Create an empty obs::property
		 */
		FORCE_INLINE property()
			: _self(nullptr)
		{
		}

		/** Encapsulate an existing obs_property_t*.
		 *
		 * Behavior is undefined if the obs_property_t* is deleted in any way.
		 */
		FORCE_INLINE property(obs_property_t* ref)
			: _self(ref)
		{
		}

		public:
		FORCE_INLINE property(property&& move) noexcept
		{
			std::swap(_self, move._self);
		}

		FORCE_INLINE ::streamfx::obs::property& operator=(property&& move) noexcept
		{
			std::swap(_self, move._self);
			return *this;
		}

		/** Only copies the reference and does not clone the content.
		 */
		FORCE_INLINE property(const property& copy)
		{
			_self = copy._self;
		}

		/** Only copies the reference and does not clone the content.
		 */
		FORCE_INLINE ::streamfx::obs::property& operator=(property& copy)
		{
			_self = copy._self;
			return *this;
		}

		public:
		FORCE_INLINE operator ::obs_property_t*() const
		{
			return _self;
		}

		FORCE_INLINE ::obs_property_t* operator*() const
		{
			return _self;
		}

		FORCE_INLINE ::obs_property_t*& operator&()
		{
			return _self;
		}

		FORCE_INLINE operator bool() const
		{
			return _self != nullptr;
		}

		FORCE_INLINE bool operator==(property const& rhs) const
		{
			return _self == rhs._self;
		}

		FORCE_INLINE bool operator<(property const& rhs) const
		{
			return _self < rhs._self;
		}

		FORCE_INLINE bool operator==(obs_property_t* rhs) const
		{
			return _self == rhs;
		}

		public:
		/** Type of property
		 */
		FORCE_INLINE ::obs_property_type type()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_get_type(_self);
		}

		/** Read the unique key identifying this property.
		 */
		FORCE_INLINE ::std::string_view key()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return ::std::string_view(obs_property_name(_self)); // This can theoretically return null, but that is invalid.
		}

		/** Human Readable Name
		 *
		 * Internally called description for some reason. 
		 */
		FORCE_INLINE ::std::optional<::std::string_view> name()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_description(_self);
		}

		/** Set the human readable name.
		 *
		 * Internally called description for some reason.
		 * The passed string is duplicated by libobs and can be freed immediately.
		 */
		FORCE_INLINE ::streamfx::obs::property& set_name(::std::optional<::std::string_view> text)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_set_description(_self, text.has_value() ? text.value().data() : nullptr);
			return *this;
		}

		/** Content of Tooltip or ? Button
		 */
		FORCE_INLINE ::std::optional<::std::string_view> description()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_long_description(_self);
		}

		/** Set Content of Tooltip or ? Button
		 *
		 * The passed string is duplicated by libobs and can be freed immediately.
		 */
		FORCE_INLINE ::streamfx::obs::property& set_description(::std::optional<::std::string_view> text)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_set_long_description(_self, text.has_value() ? text.value().data() : nullptr);
			return *this;
		}

		/** Is the property interactive?
		 */
		FORCE_INLINE bool enabled()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_enabled(_self);
		}

		/** Should the property be interactive?
		 */
		FORCE_INLINE ::streamfx::obs::property& set_enabled(bool v)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_set_enabled(_self, v);
			return *this;
		}

		/** Is the property visible?
		 */
		FORCE_INLINE bool visible()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_visible(_self);
		}

		/** Should the property be visible?
		 */
		FORCE_INLINE ::streamfx::obs::property& set_visible(bool v)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_set_visible(_self, v);
			return *this;
		}

		public /* Signals */:
		/** Signal that this property was modified.
		 *
		 * @return [bool] `true` if the UI should be reloaded
		 */
		FORCE_INLINE bool signal_modified(::obs_data_t* data)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_modified(_self, data);
		}

		FORCE_INLINE ::streamfx::obs::property& set_modified_callback(::obs_property_modified2_t callback, void* data)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_set_modified_callback2(_self, callback, data);
			return *this;
		}

		FORCE_INLINE ::streamfx::obs::property& set_modified_callback(::obs_property_modified_t callback)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_set_modified_callback(_self, callback);
			return *this;
		}

		public /* Iterator */:
		FORCE_INLINE ::streamfx::obs::property next()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_t* next = _self;
			obs_property_next(&next);
			return ::streamfx::obs::property(next);
		}
	};

	class group_property : public property {
		public:
		group_property(obs_property_t* p)
			: property(p) {}

		FORCE_INLINE ::obs_group_type type()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_group_type(_self);
		}

		::streamfx::obs::properties content();
	};

	class button_property : public property {
		public:
		button_property(obs_property_t* p)
			: property(p) {}

		FORCE_INLINE ::obs_button_type type()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_button_type(_self);
		}

		FORCE_INLINE ::streamfx::obs::button_property& set_type(::obs_button_type type)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_button_set_type(_self, type);
			return *this;
		}

		FORCE_INLINE ::std::string_view url()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_button_url(_self);
		}

		FORCE_INLINE ::streamfx::obs::button_property& set_url(::std::string_view url)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			// This is actually a const char* but libOBS forgot to add the specifier.
			obs_property_button_set_url(_self, const_cast<char*>(url.data()));
			return *this;
		}

		public /* Signals */:
		FORCE_INLINE bool clicked(void* data)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_button_clicked(_self, data);
		}
	};

	class float_property : public property {
		public:
		float_property(obs_property_t* p)
			: property(p) {}

		FORCE_INLINE ::obs_number_type type()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_float_type(_self);
		}

		FORCE_INLINE ::std::optional<::std::string_view> suffix()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_float_suffix(_self);
		}

		FORCE_INLINE ::streamfx::obs::float_property& set_suffix(::std::optional<::std::string_view> suffix)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_float_set_suffix(_self, suffix.has_value() ? suffix.value().data() : nullptr);
			return *this;
		}

		FORCE_INLINE double minimum()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_float_min(_self);
		}

		FORCE_INLINE double maximum()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_float_max(_self);
		}

		FORCE_INLINE double step()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_float_step(_self);
		}

		FORCE_INLINE ::std::pair<double, double> limits()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return {
				obs_property_float_min(_self), obs_property_float_max(_self)};
		}

		FORCE_INLINE ::streamfx::obs::float_property& set_limits(double minimum, double maximum, ::std::optional<double> step)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_float_set_limits(_self, minimum, maximum, step.value_or(this->step()));
			return *this;
		}
	};

	class int_property : public property {
		public:
		int_property(obs_property_t* p)
			: property(p) {}

		FORCE_INLINE ::obs_number_type type()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_int_type(_self);
		}

		FORCE_INLINE ::std::optional<::std::string_view> suffix()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_int_suffix(_self);
		}

		FORCE_INLINE ::streamfx::obs::int_property& set_suffix(::std::optional<::std::string_view> suffix)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_int_set_suffix(_self, suffix.has_value() ? suffix.value().data() : nullptr);
			return *this;
		}

		FORCE_INLINE int32_t minimum()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_int_min(_self);
		}

		FORCE_INLINE int32_t maximum()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_int_max(_self);
		}

		FORCE_INLINE int32_t step()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_int_step(_self);
		}

		FORCE_INLINE ::std::pair<int32_t, int32_t> limits()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return {
				obs_property_int_min(_self), obs_property_int_max(_self)};
		}

		FORCE_INLINE ::streamfx::obs::int_property& set_limits(int32_t minimum, int32_t maximum, ::std::optional<int32_t> step)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_int_set_limits(_self, minimum, maximum, step.value_or(this->step()));
			return *this;
		}
	};

	class text_property : public property {
		public:
		text_property(obs_property_t* p)
			: property(p) {}

		FORCE_INLINE ::obs_text_type type()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_text_type(_self);
		}

		FORCE_INLINE bool monospace()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_text_monospace(_self);
		}

		FORCE_INLINE ::streamfx::obs::text_property& set_monospace(bool enabled)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_text_set_monospace(_self, enabled);
			return *this;
		}
	};

	class info_text_property : public text_property {
		public:
		info_text_property(obs_property_t* p)
			: text_property(p) {}

		FORCE_INLINE ::obs_text_info_type info_type()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_text_info_type(_self);
		}

		FORCE_INLINE ::streamfx::obs::info_text_property& set_info_type(::obs_text_info_type type)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_text_set_info_type(_self, type);
			return *this;
		}

		FORCE_INLINE bool word_wrap()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_text_info_word_wrap(_self);
		}

		FORCE_INLINE ::streamfx::obs::info_text_property& set_word_wrap(bool enabled)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_text_set_info_word_wrap(_self, enabled);
			return *this;
		}
	};

	class path_property : public property {
		public:
		path_property(obs_property_t* p)
			: property(p) {}

		FORCE_INLINE ::obs_path_type type()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_path_type(_self);
		}

		::std::optional<::std::string_view> filter()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_path_filter(_self);
		}

		::std::optional<::std::string_view> default_path()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_path_default_path(_self);
		}
	};

	class list_property : public property {
		public:
		list_property(obs_property_t* p)
			: property(p) {}

		FORCE_INLINE ::obs_combo_type type()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_list_type(_self);
		}

		FORCE_INLINE ::obs_combo_format format()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_list_format(_self);
		}

		FORCE_INLINE size_t count()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_list_item_count(_self);
		}

		FORCE_INLINE size_t size()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return count();
		}

		FORCE_INLINE ::streamfx::obs::list_property& clear()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_list_clear(_self);
			return *this;
		}

		FORCE_INLINE size_t add(::std::string_view name, bool value)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_list_add_bool(_self, name.data(), value);
		}

		FORCE_INLINE size_t add(::std::string_view name, double value)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_list_add_float(_self, name.data(), value);
		}

		FORCE_INLINE size_t add(::std::string_view name, int64_t value)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_list_add_int(_self, name.data(), value);
		}

		FORCE_INLINE size_t add(::std::string_view name, ::std::optional<::std::string_view> value)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_list_add_string(_self, name.data(), value.has_value() ? value.value().data() : nullptr);
		}

		FORCE_INLINE size_t add(::std::string_view name, nullptr_t value)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_list_add_string(_self, name.data(), nullptr);
		}

		FORCE_INLINE ::streamfx::obs::list_property& insert(size_t at, ::std::string_view name, bool value)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
#ifdef DEBUG
			assert(at <= size());
#endif
			obs_property_list_insert_bool(_self, at, name.data(), value);
			return *this;
		}

		FORCE_INLINE ::streamfx::obs::list_property& insert(size_t at, ::std::string_view name, double value)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
#ifdef DEBUG
			assert(at <= size());
#endif
			obs_property_list_insert_float(_self, at, name.data(), value);
			return *this;
		}

		FORCE_INLINE ::streamfx::obs::list_property& insert(size_t at, ::std::string_view name, int64_t value)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
#ifdef DEBUG
			assert(at <= size());
#endif
			obs_property_list_insert_int(_self, at, name.data(), value);
			return *this;
		}

		FORCE_INLINE ::streamfx::obs::list_property& insert(size_t at, ::std::string_view name, ::std::optional<::std::string_view> value)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
#ifdef DEBUG
			assert(at <= size());
#endif
			obs_property_list_insert_string(_self, at, name.data(), value.has_value() ? value.value().data() : nullptr);
			return *this;
		}

		FORCE_INLINE ::streamfx::obs::list_property& insert(size_t at, ::std::string_view name, nullptr_t value)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
#ifdef DEBUG
			assert(at <= size());
#endif
			obs_property_list_insert_string(_self, at, name.data(), nullptr);
			return *this;
		}

		FORCE_INLINE bool item_bool(size_t at)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
#ifdef DEBUG
			assert(at < size());
#endif
			return obs_property_list_item_bool(_self, at);
		}

		FORCE_INLINE int64_t item_int(size_t at)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
#ifdef DEBUG
			assert(at < size());
#endif
			return obs_property_list_item_int(_self, at);
		}

		FORCE_INLINE ::std::optional<::std::string_view> item_string(size_t at)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
#ifdef DEBUG
			assert(at < size());
#endif
			return obs_property_list_item_string(_self, at);
		}

		FORCE_INLINE ::std::optional<::std::string_view> item_name(size_t at)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
#ifdef DEBUG
			assert(at < size());
#endif
			return obs_property_list_item_name(_self, at);
		}

		FORCE_INLINE bool item_disabled(size_t at)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
#ifdef DEBUG
			assert(at < size());
#endif
			return obs_property_list_item_disabled(_self, at);
		}

		FORCE_INLINE ::streamfx::obs::list_property& item_disable(size_t at, bool disabled)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
#ifdef DEBUG
			assert(at < size());
#endif
			obs_property_list_item_disable(_self, at, disabled);
			return *this;
		}

		FORCE_INLINE ::streamfx::obs::list_property& remove(size_t at)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
#ifdef DEBUG
			assert(at < size());
#endif
			obs_property_list_item_remove(_self, at);
			return *this;
		}
	};

	class editable_list_property : public property {
		public:
		editable_list_property(obs_property_t* p)
			: property(p) {}

		FORCE_INLINE ::obs_editable_list_type type()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_editable_list_type(_self);
		}

		FORCE_INLINE ::std::optional<::std::string_view> default_path()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_editable_list_default_path(_self);
		}

		FORCE_INLINE ::std::optional<::std::string_view> filter()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_editable_list_filter(_self);
		}
	};

	class frame_rate_property : public property {
		public:
		frame_rate_property(obs_property_t* p)
			: property(p) {}

		FORCE_INLINE ::streamfx::obs::frame_rate_property& clear()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_frame_rate_clear(_self);
			return *this;
		}

		public /* ranges */:
		FORCE_INLINE ::streamfx::obs::frame_rate_property& clear_ranges()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_frame_rate_fps_ranges_clear(_self);
			return *this;
		}

		FORCE_INLINE size_t count_ranges()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_frame_rate_fps_ranges_count(_self);
		}

		FORCE_INLINE size_t add_range(::media_frames_per_second min, ::media_frames_per_second max)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_frame_rate_fps_range_add(_self, min, max);
		}

		FORCE_INLINE ::streamfx::obs::frame_rate_property& insert_option(size_t at, ::media_frames_per_second min, ::media_frames_per_second max)
		{
#ifdef DEBUG
			assert(_self != nullptr);
			assert(at <= count_options());
#endif
			obs_property_frame_rate_fps_range_insert(_self, at, min, max);
		}

		FORCE_INLINE std::pair<::media_frames_per_second, ::media_frames_per_second> range_at(size_t at)
		{
#ifdef DEBUG
			assert(_self != nullptr);
			assert(at < count_options());
#endif
			return {
				obs_property_frame_rate_fps_range_min(_self, at), obs_property_frame_rate_fps_range_max(_self, at)};
		}

		public /* options */:
		FORCE_INLINE ::streamfx::obs::frame_rate_property& clear_options()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_property_frame_rate_options_clear(_self);
			return *this;
		}

		FORCE_INLINE size_t count_options()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_frame_rate_options_count(_self);
		}

		FORCE_INLINE size_t add_option(::std::string_view key, ::std::string_view name)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_property_frame_rate_option_add(_self, key.data(), name.data());
		}

		FORCE_INLINE ::streamfx::obs::frame_rate_property& insert_option(size_t at, ::std::string_view key, ::std::string_view name)
		{
#ifdef DEBUG
			assert(_self != nullptr);
			assert(at <= count_options());
#endif
			obs_property_frame_rate_option_insert(_self, at, key.data(), name.data());
			return *this;
		}

		FORCE_INLINE ::std::string_view option_key(size_t at)
		{
#ifdef DEBUG
			assert(_self != nullptr);
			assert(at < count_options());
#endif
			return obs_property_frame_rate_option_name(_self, at);
		}

		FORCE_INLINE ::std::string_view option_name(size_t at)
		{
#ifdef DEBUG
			assert(_self != nullptr);
			assert(at < count_options());
#endif
			return obs_property_frame_rate_option_description(_self, at);
		}
	};
} // namespace streamfx::obs
