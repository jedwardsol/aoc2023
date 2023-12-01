#pragma once

#include <format>
#include <string_view>

#include <iostream>
#include <iterator>


template <typename ...ARGS>
constexpr void print(std::string_view  format, ARGS    &&...args)
{
    std::ostream_iterator< char > out { std::cout };
    std::vformat_to( out, format, std::make_format_args( args... ) );
}

