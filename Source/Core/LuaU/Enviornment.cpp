#include "Enviornment.hpp"
#include "Wrapper.hpp"

#define lua_pushfunction(state, wrapper_function) lua_pushcfunction( state, wrapper_function, nullptr )

void register_global_function( lua_State* state, lua_CFunction wrapper_function, std::initializer_list<std::string> names ) {
    for ( const auto& name : names ) {
        lua_pushfunction( state, wrapper_function );
        lua_setglobal( state, name.c_str( ) );
    }
}

void register_global_table( lua_State* state, std::initializer_list<std::string> names ) {
    lua_createtable( state, 0, names.size( ) );

    for ( const auto& name : names ) {
        lua_setglobal( state, name.c_str( ) );
    }
}

void register_function( lua_State* state, lua_CFunction wrapper_function, std::initializer_list<std::string> names ) {
    for ( const auto& name : names ) {
        lua_pushfunction( state, wrapper_function );
        lua_setfield( state, -2, name.c_str( ) );
    }
}

Enviornment::Enviornment( ) {
    m_State = luaL_newstate( );

    if ( !m_State )
        throw std::runtime_error( "Failed to initialize Lua state." );

    luaL_openlibs( m_State );

    {
        luaL_newmetatable( m_State, "Vector2" );

        register_function( m_State, Wrapper::Types::Vector2_::__index, { "__index" } );
        register_function( m_State, Wrapper::Types::Vector2_::__newindex, { "__newindex" } );

        lua_pop( m_State, 1 );

        register_global_function( m_State, Wrapper::Types::Vector2_::New, { "Vector2", "vector2" } );
    }

    {
        luaL_newmetatable( m_State, "Vector3" );

        register_function( m_State, Wrapper::Types::Vector3_::__index, { "__index" } );
        register_function( m_State, Wrapper::Types::Vector3_::__newindex, { "__newindex" } );

        lua_pop( m_State, 1 );

        register_global_function( m_State, Wrapper::Types::Vector3_::New, { "Vector3", "vector3" } );
    }

    {
        luaL_newmetatable( m_State, "Vector4" );

        register_function( m_State, Wrapper::Types::Vector4_::__index, { "__index" } );
        register_function( m_State, Wrapper::Types::Vector4_::__newindex, { "__newindex" } );

        lua_pop( m_State, 1 );

        register_global_function( m_State, Wrapper::Types::Vector4_::New, { "Vector4", "vector4" } );
    }

    {
        luaL_newmetatable( m_State, "Vertex" );

        register_function( m_State, Wrapper::Types::Vertex_::__index, { "__index" } );
        register_function( m_State, Wrapper::Types::Vertex_::__newindex, { "__newindex" } );

        lua_pop( m_State, 1 );

        register_global_function( m_State, Wrapper::Types::Vertex_::New, { "Vertex", "vertex" } );
    }

    {
        luaL_newmetatable( m_State, "Color" );

        register_function( m_State, Wrapper::Types::Color_::__index, { "__index" } );
        register_function( m_State, Wrapper::Types::Color_::__newindex, { "__newindex" } );

        lua_pop( m_State, 1 );

        register_global_function( m_State, Wrapper::Types::Color_::New, { "Color", "color" } );
    }

    {
        lua_createtable( m_State, 0, 6 );

        register_function( m_State, Wrapper::Win32_::CreateWindow_, { "CreateWindow", "create_window" } );
        register_function( m_State, Wrapper::Win32_::DestroyWindow, { "DestroyWindow", "destroy_window" } );
        register_function( m_State, Wrapper::Win32_::CreateConsole, { "CreateConsole", "create_console" } );
        register_function( m_State, Wrapper::Win32_::DestroyConsole, { "DestroyConsole", "destroy_console" } );
        register_function( m_State, Wrapper::Win32_::MessageBox_, { "CreateMessageBox", "create_message_box" } );
        register_function( m_State, Wrapper::Win32_::GetScreenSize, { "GetScreenSize", "get_screen_size" } );

        register_global_table( m_State, { "Win32", "win32" } );
    }

    register_global_function( m_State, Wrapper::Utils_::RandomString, { "RandomString", "random_string" } );

    register_global_function( m_State, Wrapper::Utils_::SetClipboard, { "SetClipboard", "set_clipboard" } );
    register_global_function( m_State, Wrapper::Utils_::GetClipboard, { "GetClipboard", "get_clipboard" } );

    register_global_function( m_State, Wrapper::FileSystem_::ReadFile, { "ReadFile", "read_file" } );
    register_global_function( m_State, Wrapper::FileSystem_::WriteFile, { "WriteFile", "write_file" } );

    register_global_function( m_State, Wrapper::Http_::Get, { "HttpGet", "http_get" } );
    register_global_function( m_State, Wrapper::Http_::Post, { "HttpPost", "http_post" } );
    register_global_function( m_State, Wrapper::Http_::Put, { "HttpPut", "http_put" } );
    register_global_function( m_State, Wrapper::Http_::Delete, { "HttpDelete", "http_delete" } );
}

Enviornment::~Enviornment() {
    if (m_State) {
        lua_close(m_State);
    }
}

bool Enviornment::LoadScript(const std::string& script) {
    size_t BytecodeSize = 0;
    char* Bytecode = luau_compile(script.c_str(), script.size(), nullptr, &BytecodeSize);

    int Result = luau_load(m_State, "main chunk", Bytecode, BytecodeSize, 0);

    free(Bytecode);

    if (Result != LUA_OK) {
        HandleError(Result);
        return false;
    }

    Result = lua_pcall(m_State, 0, LUA_MULTRET, 0);

    if (Result != LUA_OK) {
        HandleError(Result);
        return false;
    }

    return true;
}

lua_State* Enviornment::GetState() const {
    return m_State;
}

// PRIVATE

void Enviornment::HandleError(int error_code) {
    const char* Error = lua_tostring(m_State, -1);
    std::cerr << "[" << error_code << "] " << Error << std::endl;

    lua_pop(m_State, 1);
}