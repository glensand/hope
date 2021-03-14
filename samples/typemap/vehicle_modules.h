/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

namespace hope::sample::typemap {

    class vehicle_module {
    public:
        virtual ~vehicle_module() = default;
        virtual std::string_view name() const noexcept = 0;
    };

#define DECLARE_INTERFACE(name) class name : public vehicle_module {\
    public:\
        name() = default;\
     };\

#define DECLARE_MODULE(class_name, interface_name) class class_name final : public interface_name {\
    public:\
        class_name() = default;\
        virtual std::string_view name() const noexcept override {\
            return #class_name;\
        }\
    };\

DECLARE_INTERFACE(graphics)
DECLARE_INTERFACE(logic)
DECLARE_INTERFACE(physics)

DECLARE_MODULE(vehicle_graphics, graphics)
DECLARE_MODULE(vehicle_logic, logic)
DECLARE_MODULE(vehicle_physics, physics)

DECLARE_MODULE(legacy_graphics, graphics)
DECLARE_MODULE(legacy_logic, logic)
DECLARE_MODULE(legacy_physics, physics)

#undef DECLARE_MODULE
#undef DECLARE_INTERFACE

}
