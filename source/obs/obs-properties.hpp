
#pragma once
#include "common.hpp"
#include "obs-property.hpp"
#include "plugin.hpp"

#include "warning-disable.hpp"
#include <memory>
#include <string>
#include <string_view>
#include "warning-enable.hpp"

namespace streamfx::obs {
	class properties {
		std::shared_ptr<obs_properties_t> _self;

		public:
		FORCE_INLINE ~properties()
		{
			_self.reset();
		}

		/** Wrap an existing obs_properties_t* with optional ownership transfer.
		 */
		FORCE_INLINE properties(obs_properties_t* ref, bool owner = false)
		{
			if (owner) {
				_self = decltype(_self){ref,
										[](obs_properties_t* ref) { obs_properties_destroy(ref); }};
			} else {
				_self = decltype(_self){
					ref, [](obs_properties_t*) {}}; // Explicitly define a no-op destructor.
			}
		}

		/** Create an empty instance.
		 *
		 * This will return nullptr or false where appropriate. It is illegal to perform any operations on this object.
		 */
		FORCE_INLINE properties(nullptr_t ref)
			: _self()
		{
		}

		/** Create a blank new obs_properties_t* object.
		 *
		 * Optionally can take ownership of the empty properties object if necessary.
		 */
		FORCE_INLINE properties(bool owner = false)
		{
			if (owner) {
				_self = decltype(_self){
					obs_properties_create(), [](obs_properties_t* ref) { obs_properties_destroy(ref); }};
			} else {
				_self = decltype(_self){
					obs_properties_create(), [](obs_properties_t* ref) {}};
			}
		}

		public:
		FORCE_INLINE properties(::streamfx::obs::properties&& move) noexcept
		{
			std::swap(_self, move._self);
		}

		FORCE_INLINE ::streamfx::obs::properties& operator=(::streamfx::obs::properties&& move) noexcept
		{
			std::swap(_self, move._self);

			return *this;
		}

		/** Only copies the reference and does not clone the content.
		 */
		FORCE_INLINE properties(const ::streamfx::obs::properties& copy)
		{
			_self = copy._self;
		}

		/** Only copies the reference and does not clone the content.
		 */
		FORCE_INLINE ::streamfx::obs::properties& operator=(::streamfx::obs::properties& copy)
		{
			_self = copy._self;
			return *this;
		}

		public:
		FORCE_INLINE operator obs_properties_t*() const
		{
			return _self.get();
		}

		FORCE_INLINE obs_properties_t* operator*() const
		{
			return _self.get();
		}

		FORCE_INLINE operator bool() const
		{
			return _self != nullptr;
		}

		FORCE_INLINE bool operator==(::streamfx::obs::properties const& rhs) const
		{
			return _self == rhs._self;
		}

		FORCE_INLINE bool operator<(::streamfx::obs::properties const& rhs) const
		{
			return _self < rhs._self;
		}

		FORCE_INLINE bool operator==(obs_properties_t* rhs) const
		{
			return _self.get() == rhs;
		}

		public:
		/** Retrieve the parent obs_properties_t* that contains this.
		 *
		 * @return A valid properties object (we do not take ownership).
		 */
		FORCE_INLINE ::streamfx::obs::properties parent()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return ::streamfx::obs::properties(obs_properties_get_parent(*this));
		}

		public /* Iteration */:
		/** Find a property by the given unique key.
		 *
		 * Will return an empty property if said key does not exist.
		 */
		FORCE_INLINE ::streamfx::obs::property find(std::string_view key)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return ::streamfx::obs::property(obs_properties_get(*this, key.data()));
		}

		FORCE_INLINE::streamfx::obs::property operator[](std::string_view key)
		{
			return this->find(key);
		}

		/** Retrieve the first property in this object.
		 *
		 * Will return an empty property if there are no properties.
		 */
		FORCE_INLINE ::streamfx::obs::property first()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return ::streamfx::obs::property(obs_properties_first(*this));
		}

		public /* Manipulation */:
		/** Remove a property by its key
		 *
		 * - May not be called outside of get_properties, get_properties2, modified_callback, or modified_callback2.
		 * - It is the callers responsibility to ensure that any references to the same obs_property_t* are properly invalidated.
		 */
		void remove(std::string_view const key)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_properties_remove_by_name(*this, key.data());
		}

		/** Add a new group property which contains more properties.
		 *
		 * 
		 */
		::streamfx::obs::group_property add_group(std::string_view const key, std::string_view const name, obs_group_type type, streamfx::obs::properties group)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return ::streamfx::obs::group_property(obs_properties_add_group(*this, key.data(), name.data(), type, group));
		}

		/** Add a checkbox property.
		 */
		::streamfx::obs::property add_bool(std::string_view const key, std::string_view const name)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return ::streamfx::obs::property(obs_properties_add_bool(*this, key.data(), name.data()));
		}

		::streamfx::obs::button_property add_button(std::string_view const key, std::string_view const name, obs_property_clicked_t callback, void* callback_param = nullptr)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return ::streamfx::obs::button_property(obs_properties_add_button2(*this, key.data(), name.data(), callback, callback_param));
		}

		::streamfx::obs::property add_color(std::string_view const key, std::string_view const name, bool alpha)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			if (alpha) {
				return ::streamfx::obs::property(obs_properties_add_color_alpha(*this, key.data(), name.data()));
			} else {
				return ::streamfx::obs::property(obs_properties_add_color(*this, key.data(), name.data()));
			}
		}

		::streamfx::obs::list_property add_list(std::string_view const key, std::string_view const name, obs_combo_type type, obs_combo_format format)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return ::streamfx::obs::list_property(obs_properties_add_list(*this, key.data(), name.data(), type, format));
		}

		::streamfx::obs::editable_list_property add_editable_list(std::string_view const key, std::string_view const name, obs_editable_list_type type, std::optional<std::string_view const> filter = nullptr, std::optional<std::string_view const> default_path = nullptr)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return ::streamfx::obs::editable_list_property(obs_properties_add_editable_list(*this, key.data(), name.data(), type, filter.has_value() ? filter.value().data() : nullptr, default_path.has_value() ? default_path.value().data() : nullptr));
		}

		::streamfx::obs::float_property add_float(std::string_view const key, std::string_view const name, double minimum, double maximum, double step, bool slider = false)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			if (slider) {
				return ::streamfx::obs::float_property(obs_properties_add_float_slider(*this, key.data(), name.data(), minimum, maximum, step));
			} else {
				return ::streamfx::obs::float_property(obs_properties_add_float(*this, key.data(), name.data(), minimum, maximum, step));
			}
		}

		::streamfx::obs::int_property add_int(std::string_view const key, std::string_view const name, int32_t minimum, int32_t maximum, int32_t step, bool slider = false)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			if (slider) {
				return ::streamfx::obs::int_property(obs_properties_add_int_slider(*this, key.data(), name.data(), minimum, maximum, step));
			} else {
				return ::streamfx::obs::int_property(obs_properties_add_int(*this, key.data(), name.data(), minimum, maximum, step));
			}
		}

		::streamfx::obs::path_property add_path(std::string_view const key, std::string_view const name, obs_path_type type, std::optional<std::string_view const> filter = nullptr, std::optional<std::string_view const> default_path = nullptr)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return ::streamfx::obs::path_property(obs_properties_add_path(*this, key.data(), name.data(), type, filter.has_value() ? filter.value().data() : nullptr, default_path.has_value() ? default_path.value().data() : nullptr));
		}

		::streamfx::obs::text_property add_text(std::string_view const key, std::string_view const name, obs_text_type type)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return ::streamfx::obs::text_property(obs_properties_add_text(*this, key.data(), name.data(), type));
		}

		::streamfx::obs::info_text_property add_info_text(std::string_view const key, std::string_view const name)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return ::streamfx::obs::info_text_property(obs_properties_add_text(*this, key.data(), name.data(), ::obs_text_type::OBS_TEXT_INFO));
		}

		public:
		uint32_t get_flags()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_properties_get_flags(*this);
		}

		void set_flags(uint32_t flags)
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_properties_set_flags(*this, flags);
		}

		void* get_param()
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			return obs_properties_get_param(*this);
		}

		void set_param(void* param, void (*destructor)(void* param))
		{
#ifdef DEBUG
			assert(_self != nullptr);
#endif
			obs_properties_set_param(*this, param, destructor);
		}
	};
} // namespace streamfx::obs
