//
// Created by gomkyung2 on 12/1/23.
//

#pragma once

#include <tuple>

namespace type_mapper{
    template <typename KeyT, typename ValueT>
    struct mapping{
        using type = KeyT;
        ValueT value;
    };

    template <typename KeyT, typename ValueT>
    constexpr mapping<KeyT, ValueT> make_mapping(ValueT &&value) noexcept{
        return { std::forward<ValueT>(value) };
    }

    template <typename MappingV, typename... MappingTs>
    struct mapper{
        std::tuple<mapping<MappingTs, MappingV>...> mappings;

        explicit constexpr mapper(mapping<MappingTs, MappingV> &&...mappings)
            noexcept((noexcept(mapping<MappingTs, MappingV> { std::move(mappings) }) && ...))
            : mappings { std::move(mappings)... }
        { }

        template <typename T>
        constexpr MappingV &get() noexcept {
            return std::get<mapping<T, MappingV>>(mappings).value;
        }

        template <typename T>
        constexpr const MappingV &get() const noexcept {
            return std::get<mapping<T, MappingV>>(mappings).value;
        }
    };
}