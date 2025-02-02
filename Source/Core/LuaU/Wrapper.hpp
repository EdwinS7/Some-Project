#ifndef LUAU_RUNTIME_H
#define LUAU_RUNTIME_H

#include "../Common.hpp"
#include "../Memory/Memory.hpp"
#include "../FileSystem/FileSystem.hpp"
#include "../Utils/Utils.hpp"
#include "../Win32/Win32.hpp"
#include "../Http/Http.hpp"
#include "../Graphics/Types.hpp"

// NOTE: Just here so I can copy it for creating new stuff
int Dummy( lua_State* L ) {
    return 1;
}

namespace Wrapper {
    namespace Types {
        namespace Vector2_ {
            int New( lua_State* state ) {
                auto Vector = ( Vector2* ) lua_newuserdata( state, sizeof( Vector2 ) );

                new ( Vector ) Vector2(
                    luaL_optnumber( state, 1, 0.0 ),
                    luaL_optnumber( state, 2, 0.0 )
                );

                luaL_getmetatable( state, "Vector2" );
                lua_setmetatable( state, -2 );

                return 1;
            }

            int __index( lua_State* state ) {
                auto Vector = ( Vector2* ) luaL_checkudata( state, 1, "Vector2" );
                lua_pushnumber( state, luaL_checkstring( state, 2 )[ 0 ] == 'x' ? Vector->x : Vector->y );

                return 1;
            }

            int __newindex( lua_State* state ) {
                auto Vector = ( Vector2* ) luaL_checkudata( state, 1, "Vector2" );
                
                auto Key = luaL_checkstring( state, 2 );
                auto Value = luaL_checknumber( state, 3 );

                if ( Key[ 0 ] == 'x' ) 
                    Vector->x = Value;
                else if ( Key[ 0 ] == 'y' ) 
                    Vector->y = Value;

                return 0;
            }
        }

        namespace Vector3_ {
            int New( lua_State* state ) {
                auto Vector = ( Vector3* ) lua_newuserdata( state, sizeof( Vector3 ) );

                new ( Vector ) Vector3(
                    luaL_optnumber( state, 1, 0.0 ),
                    luaL_optnumber( state, 2, 0.0 ),
                    luaL_optnumber( state, 3, 0.0 )
                );

                luaL_getmetatable( state, "Vector3" );
                lua_setmetatable( state, -2 );

                return 1;
            }

            int __index( lua_State* state ) {
                auto Vector = ( Vector3* ) luaL_checkudata( state, 1, "Vector3" );
                auto Key = luaL_checkstring( state, 2 );

                if ( Key[ 0 ] == 'x' )
                    lua_pushnumber( state, Vector->x );
                else if ( Key[ 0 ] == 'y' )
                    lua_pushnumber( state, Vector->y );
                else if ( Key[ 0 ] == 'z' )
                    lua_pushnumber( state, Vector->z );

                return 1;
            }

            int __newindex( lua_State* state ) {
                auto Vector = ( Vector3* ) luaL_checkudata( state, 1, "Vector3" );

                auto Key = luaL_checkstring( state, 2 );
                auto Value = luaL_checknumber( state, 3 );

                if ( Key[ 0 ] == 'x' )
                    Vector->x = Value;
                else if ( Key[ 0 ] == 'y' )
                    Vector->y = Value;
                else if ( Key[ 0 ] == 'z' )
                    Vector->z = Value;

                return 0;
            }
        }

        namespace Vector4_ {
            int New( lua_State* state ) {
                auto Vector = ( Vector4* ) lua_newuserdata( state, sizeof( Vector4 ) );

                new ( Vector ) Vector4(
                    luaL_optnumber( state, 1, 0.0 ),
                    luaL_optnumber( state, 2, 0.0 ),
                    luaL_optnumber( state, 3, 0.0 ),
                    luaL_optnumber( state, 4, 0.0 )
                );

                luaL_getmetatable( state, "Vector4" );
                lua_setmetatable( state, -2 );

                return 1;
            }

            int __index( lua_State* state ) {
                auto Vector = ( Vector4* ) luaL_checkudata( state, 1, "Vector4" );
                auto Key = luaL_checkstring( state, 2 );

                if ( Key[ 0 ] == 'x' )
                    lua_pushnumber( state, Vector->x );
                else if ( Key[ 0 ] == 'y' )
                    lua_pushnumber( state, Vector->y );
                else if ( Key[ 0 ] == 'z' )
                    lua_pushnumber( state, Vector->z );
                else if ( Key[ 0 ] == 'w' )
                    lua_pushnumber( state, Vector->w );

                return 1;
            }

            int __newindex( lua_State* state ) {
                auto Vector = ( Vector4* ) luaL_checkudata( state, 1, "Vector4" );
                auto Key = luaL_checkstring( state, 2 );
                auto Value = luaL_checknumber( state, 3 );

                if ( Key[ 0 ] == 'x' )
                    Vector->x = Value;
                else if ( Key[ 0 ] == 'y' )
                    Vector->y = Value;
                else if ( Key[ 0 ] == 'z' )
                    Vector->z = Value;
                else if ( Key[ 0 ] == 'w' )
                    Vector->w = Value;

                return 0;
            }
        }

        namespace Vertex_ {
            int New( lua_State* state ) {
                auto Vertex_ = ( Vertex* ) lua_newuserdata( state, sizeof( Vertex ) );

                new ( Vertex_ ) Vertex(
                    luaL_optnumber( state, 1, 0.0 ),
                    luaL_optnumber( state, 2, 0.0 ),
                    luaL_optnumber( state, 3, 0.0 ),
                    luaL_optnumber( state, 4, 0.0 ),
                    luaL_optnumber( state, 5, 0.0 ),
                    luaL_optnumber( state, 6, 1.0 )
                );

                luaL_getmetatable( state, "Vertex" );
                lua_setmetatable( state, -2 );

                return 1;
            }

            int __index( lua_State* state ) {
                auto Vertex_ = ( Vertex* ) luaL_checkudata( state, 1, "Vertex" );
                auto Key = luaL_checkstring( state, 2 );

                if ( Key[ 0 ] == 'x' )
                    lua_pushnumber( state, Vertex_->x );
                else if ( Key[ 0 ] == 'y' )
                    lua_pushnumber( state, Vertex_->y );
                else if ( Key[ 0 ] == 'z' )
                    lua_pushnumber( state, Vertex_->z );
                else if ( Key[ 0 ] == 'u' )
                    lua_pushnumber( state, Vertex_->u );
                else if ( Key[ 0 ] == 'v' )
                    lua_pushnumber( state, Vertex_->v );
                else if ( Key == "color" ) // color
                    lua_pushnumber( state, Vertex_->color );

                return 1;
            }

            int __newindex( lua_State* state ) {
                auto Vertex_ = ( Vertex* ) luaL_checkudata( state, 1, "Vertex" );
                auto Key = luaL_checkstring( state, 2 );
                auto Value = luaL_checknumber( state, 3 );

                if ( Key[ 0 ] == 'x' )
                    Vertex_->x = Value;
                else if ( Key[ 0 ] == 'y' )
                    Vertex_->y = Value;
                else if ( Key[ 0 ] == 'z' )
                    Vertex_->z = Value;
                else if ( Key[ 0 ] == 'u' )
                    Vertex_->u = Value;
                else if ( Key[ 0 ] == 'v' )
                    Vertex_->v = Value;
                else if ( Key == "color" )
                    Vertex_->color = Value;

                return 0;
            }
        }

        namespace Color_ {
            int New( lua_State* L ) {
                auto Clr = ( Color* ) lua_newuserdata( L, sizeof( Color ) );

                new ( Clr ) Color(
                    luaL_optnumber( L, 1, 0.0 ),
                    luaL_optnumber( L, 2, 0.0 ),
                    luaL_optnumber( L, 3, 0.0 ),
                    luaL_optnumber( L, 4, 1.0 ) // Default alpha to 1.0
                );

                luaL_getmetatable( L, "Color" );
                lua_setmetatable( L, -2 );

                return 1;
            }

            int __index( lua_State* state ) {
                auto Clr = ( Color* ) luaL_checkudata( state, 1, "Color" );
                auto Key = luaL_checkstring( state, 2 );

                if ( Key[ 0 ] == 'r' )
                    lua_pushnumber( state, Clr->r );
                else if ( Key[ 0 ] == 'g' )
                    lua_pushnumber( state, Clr->g );
                else if ( Key[ 0 ] == 'b' )
                    lua_pushnumber( state, Clr->b );
                else if ( Key[ 0 ] == 'a' )
                    lua_pushnumber( state, Clr->a );

                return 1;
            }

            int __newindex( lua_State* state ) {
                auto Clr = ( Color* ) luaL_checkudata( state, 1, "Color" );

                auto Key = luaL_checkstring( state, 2 );
                auto Value = luaL_checknumber( state, 3 );

                if ( Key[ 0 ] == 'r' )
                    Clr->r = Value;
                else if ( Key[ 0 ] == 'g' )
                    Clr->g = Value;
                else if ( Key[ 0 ] == 'b' )
                    Clr->b = Value;
                else if ( Key[ 0 ] == 'a' )
                    Clr->a = Value;

                return 0;
            }
        }
    }

    namespace Utils_ {
        int RandomString( lua_State* L ) {
            luaL_checktype( L, 1, LUA_TNUMBER );

            lua_pushstring( L, Utils::RandomString( luaL_checkinteger( L, 1 ) ) );

            return 0;
        }

        int SetClipboard( lua_State* L ) {
            Utils::SetClipboard( luaL_checkstring( L, 1 ) );

            return 0;
        }

        int GetClipboard( lua_State* L ) {
            lua_pushstring( L, Utils::GetClipboard( ) );

            return 0;
        }
    }

    namespace FileSystem_ {
        int ReadFile( lua_State* L ) {
            luaL_checktype( L, 1, LUA_TSTRING );
            lua_pushstring( L, FileSystem::ReadFile( luaL_checkstring( L, 1 ) ).c_str( ) );

            return 0;
        }

        int WriteFile( lua_State* L ) {
            luaL_checktype( L, 1, LUA_TSTRING );
            luaL_checktype( L, 2, LUA_TSTRING );

            FileSystem::WriteFile( luaL_checkstring( L, 1 ), luaL_checkstring( L, 2 ) );

            return 0;
        }
    }

    namespace Win32_ {
        std::unordered_map<int, HWND> WindowHandles;
        std::unordered_map<int, HWND> ConsoleHandles;

        int CreateWindow_( lua_State* L ) {
            HWND WindowHandle = Win32::CreateWindow_( lua_tostring( L, 1 ),
                lua_tointeger( L, 2 ), lua_tointeger( L, 3 ), lua_toboolean( L, 4 )
            );

            if ( WindowHandle ) {
                int WindowRef = WindowHandles.size( ) + 1;
                WindowHandles[ WindowRef ] = WindowHandle;

                lua_pushinteger( L, WindowRef );
            }
            else {
                lua_pushnil( L );
            }

            return 1;
        }

        int DestroyWindow( lua_State* L ) {
            HWND WindowHandle = WindowHandles[ luaL_checkinteger( L, 1 ) ];

            if ( !WindowHandle )
                return 1;

            Win32::DestroyWindow( WindowHandle );

            return 0;
        }

        int CreateConsole( lua_State* L ) {
            HWND ConsoleHandle = Win32::CreateConsole( lua_tostring( L, 1 ), 
                lua_tointeger( L, 2 ), lua_tointeger( L, 3 ),
                lua_tointeger( L, 4 ), lua_tointeger( L, 5 )
            );

            if ( ConsoleHandle ) {
                int ConsoleRef = ConsoleHandles.size( ) + 1;
                ConsoleHandles[ ConsoleRef ] = ConsoleHandle;

                lua_pushinteger( L, ConsoleRef );
            }
            else {
                lua_pushnil( L );
            }

            return 1;
        }

        int DestroyConsole( lua_State* L ) {
            HWND ConsoleHandle = ConsoleHandles[ luaL_checkinteger( L, 1 ) ];

            if ( !ConsoleHandle )
                return 1;

            Win32::DestroyConsole( ConsoleHandle );

            return 0;
        }

        int MessageBox_( lua_State* L ) {
            int Reference = luaL_checkinteger( L, 1 );

            HWND WindowHandle = WindowHandles[ Reference ];
            HWND ConsoleHandle = ConsoleHandles[ Reference ];
            HWND Handle = WindowHandle ? WindowHandle : ConsoleHandle;

            if ( !Handle )
                return 1;

            Win32::MessageBox_( Handle, luaL_checkstring( L, 2 ), luaL_checkstring( L, 3 ), luaL_checkinteger( L, 4 ) );

            return 0;
        }

        int GetScreenSize( lua_State* L ) {
            Vector2 ScreenSize = Win32::GetScreenSize( );

            auto Vector = ( Vector2* ) lua_newuserdata( L, sizeof( Vector2 ) );
            
            new ( Vector ) Vector2(
                static_cast< double >( ScreenSize.x ),
                static_cast< double >( ScreenSize.y )
            );

            luaL_getmetatable( L, "Vector2" );
            lua_setmetatable( L, -2 );

            return 1;
        }
    }

    namespace Http_ {
        int Get( lua_State* L ) {
            lua_pushstring( L, Http::Get( luaL_checkstring( L, 1 ) ).c_str() );

            return 0;
        }

        int Post( lua_State* L ) {
            lua_pushstring( L, Http::Post( luaL_checkstring( L, 1 ), luaL_checkstring( L, 2 ) ).c_str( ) );

            return 0;
        }

        int Put( lua_State* L ) {
            lua_pushstring( L, Http::Put( luaL_checkstring( L, 1 ), luaL_checkstring( L, 2 ) ).c_str( ) );

            return 0;
        }

        int Delete( lua_State* L ) {
            lua_pushstring( L, Http::Delete( luaL_checkstring( L, 1 ) ).c_str( ) );

            return 0;
        }
    }
}

#endif