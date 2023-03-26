/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2022 Bernardo Giordano, Admiral Fish, piepie62
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
 *       * Requiring preservation of specified reasonable legal notices or
 *         author attributions in that material or in the Appropriate Legal
 *         Notices displayed by works containing it.
 *       * Prohibiting misrepresentation of the origin of that material,
 *         or requiring that modified versions of such material be marked in
 *         reasonable ways as different from the original version.
 */

#ifndef MAP_MACRO_HPP
#define MAP_MACRO_HPP

#define MAP_PARENS ()

#define MAP_EXPAND(...) MAP_EXPAND4(MAP_EXPAND4(MAP_EXPAND4(MAP_EXPAND4(__VA_ARGS__))))
#define MAP_EXPAND4(...) MAP_EXPAND3(MAP_EXPAND3(MAP_EXPAND3(MAP_EXPAND3(__VA_ARGS__))))
#define MAP_EXPAND3(...) MAP_EXPAND2(MAP_EXPAND2(MAP_EXPAND2(MAP_EXPAND2(__VA_ARGS__))))
#define MAP_EXPAND2(...) MAP_EXPAND1(MAP_EXPAND1(MAP_EXPAND1(MAP_EXPAND1(__VA_ARGS__))))
#define MAP_EXPAND1(...) __VA_ARGS__

#define MAP(macro, ...) __VA_OPT__(MAP_EXPAND(MAP_HELPER(macro, __VA_ARGS__)))
#define MAP_HELPER(macro, a1, ...) macro(a1) __VA_OPT__(MAP_AGAIN MAP_PARENS(macro, __VA_ARGS__))
#define MAP_AGAIN() MAP_HELPER

#define MAP_LIST(macro, ...) __VA_OPT__(MAP_EXPAND(MAP_LIST_HELPER(macro, __VA_ARGS__)))
#define MAP_LIST_HELPER(macro, a1, ...)                                                            \
    macro(a1) __VA_OPT__(, MAP_LIST_AGAIN MAP_PARENS(macro, __VA_ARGS__))
#define MAP_LIST_AGAIN() MAP_LIST_HELPER

#endif
