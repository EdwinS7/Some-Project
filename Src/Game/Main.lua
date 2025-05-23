local screen_size = win32.get_screen_size( )

do -- Console
    local console_size = vector2.new(
        screen_size.x / 1.5,
        screen_size.y / 1.5
    )
    
    local console_pos = vector2.new(
        (screen_size.x - console_size.x) / 2,
        (screen_size.y - console_size.x / 2) / 2
    )

    local console_reference = win32.create_console(
        "Impulse - Console",
        console_pos, 
        console_size
    )

    original_print = print

    print = function(...)
        local args = {...}

        for i = 1, #args do
            args[i] = tostring(args[i])
        end

        return original_print(
            console_reference, 
            table.unpack(args)
        )
    end

    print("[Impulse] Hello World!")
end

do -- Create window and graphics device
    local window_size = vector2.new(
        screen_size.x / 1.5, 
        screen_size.y / 1.5
    )
    
    local window_pos = vector2.new(
        (screen_size.x - window_size.x) / 2,
        (screen_size.y - window_size.x / 2) / 2
    )

    local window_reference, client_size = win32.create_window(
        "Impulse - DirectX 11 Demo", 
        window_pos, 
        window_size,
        false
    )

    local swap_chain_desc = {
        output_window = window_reference,
        windowed = true,

        buffer_format = DXGI_FORMAT.R8G8B8A8_UNORM,
        buffer_usage = DXGI_USAGE.RENDER_TARGET_OUTPUT,
        
        sample_count = 1,
        buffer_count = 2,

        buffer_width = client_size.x,
        buffer_height = client_size.y,

        swap_effect = DXGI_SWAP_EFFECT.DISCARD
    }

    local float_size = 4

    local buffer_desc = {
        byte_width = 4 * 4 * float_size,
        usage = D3D11_USAGE.DYNAMIC,
        bind_flags = D3D11_BIND_FLAG.CONSTANT_BUFFER,
        cpu_access_flags = D3D11_CPU_ACCESS_FLAG.WRITE,
        misc_flags = 0
    }

    local rasterizer_desc = {
        fill_mode = D3D11_FILL_MODE.SOLID,
        cull_mode = D3D11_CULL_MODE.BACK,

        scissor_enable = false, -- TODO: Add clipping.
        depth_clip_enable = true
    }

    local depth_stencil_desc = {
        depth_enable = true,

        depth_write_mask = D3D11_DEPTH_WRITE_MASK.ZERO, 
        depth_func = D3D11_COMPARISON_FUNC.ALWAYS,

        stencil_enable = false,
        
        front_face = {
            stencil_fail_op = D3D11_STENCIL_OP.KEEP,
            stencil_depth_fail_op = D3D11_STENCIL_OP.KEEP,
            stencil_pass_op = D3D11_STENCIL_OP.KEEP,
            stencil_func = D3D11_COMPARISON_FUNC.ALWAYS
        }
    }

    local sampler_desc = {
        filter = D3D11_FILTER.MIN_MAG_MIP_LINEAR,
        address_u = D3D11_TEXTURE_ADDRESS_MODE.CLAMP,
        address_v = D3D11_TEXTURE_ADDRESS_MODE.CLAMP,
        address_w = D3D11_TEXTURE_ADDRESS_MODE.CLAMP,
        mip_lod_bias = 0.0,
        comparison_func = D3D11_COMPARISON_FUNC.ALWAYS,
        min_lod = 0.0,
        max_lod = 0.0
    }

    local blend_desc = {
        alpha_to_coverage_enable = false,
        independent_blend_enable = false,

        render_target = {
            blend_enable = true,
            
            src_blend = D3D11_BLEND.SRC_ALPHA,
            dest_blend = D3D11_BLEND.INV_SRC_ALPHA,
            blend_op = D3D11_BLEND_OP.ADD,
            src_blend_alpha = D3D11_BLEND.ONE,
            dest_blend_alpha = D3D11_BLEND.INV_SRC_ALPHA,
            blend_op_alpha = D3D11_BLEND_OP.ADD,
            render_target_write_mask = D3D11_COLOR_WRITE_ENABLE.ALL
        }
    }

    local vertex_shader_src: string = file_system.read_file("Game/Shaders/vertex_shader.hlsl")
    local pixel_shader_src: string = file_system.read_file("Game/Shaders/pixel_shader.hlsl")

    local success = graphics.initiate(
        window_reference,
        swap_chain_desc,
        buffer_desc,
        rasterizer_desc,
        depth_stencil_desc,
        sampler_desc,
        blend_desc,
        vertex_shader_src,
        pixel_shader_src
    )
    
    if not success then
        error("Failed to initiate graphics.")
    end

    local fov = math.rad(60)
    local aspect = screen_size.x / screen_size.y
    local near, far = 0.1, 100.0
    local f = 1.0 / math.tan(fov / 2)

    graphics.set_projection_matrix({
        f / aspect, 0,      0,                              0,
        0,          -f,     0,                              0,
        0,          0,      far / (far - near),             1,
        0,          0,      (-far * near) / (far - near),   0,
    })
end

do -- Rendering
    local font = graphics.create_font(
        "Game/Assets/arial.ttf",    -- path
        16                          -- size
        -- TODO: add padding
    )

    local pixel_data, width, height = file_system.load_image("Game/Assets/CDazz.png")

    --[[local example_texture = graphics.create_texture(
        "cdazz",

        {
            width = width,
            height = height,
            mip_levels = 1,
            array_size = 1,
            format = DXGI_FORMAT.R8G8B8A8_UNORM,
            sample_desc = {count = 1, quality = 0},
            usage = D3D11_USAGE.DEFAULT,
            bind_flags = D3D11_BIND_FLAG.SHADER_RESOURCE,
            cpu_access_flags = 0,
            misc_flags  = 0,
        },

        {
            p_sys_mem = pixel_data,
            sys_mem_pitch = width * 4,
            sys_mem_slice_pitch = 0,
        },

        {
            format = DXGI_FORMAT.R8G8B8A8_UNORM,
            view_dimension = D3D11_SRV_DIMENSION.TEXTURE2D,

            union = {
                texture2d = {
                    most_detailed_mip = 0,
                    mip_levels = 1,
                }
            }
        }
    )]]

    local rect = renderer.rectangle(
        vector2.new(200, 200),
        vector2.new(500, 500),
        color.new(25, 25, 25, 255),
        true, nil, 1
    )

    local text = renderer.text(
        font, "Impulse - demo",
        vector2.new(rect.position.x + 3, rect.position.y + 3),
        color.new(255, 255, 255, 255),
        rect.z_index + 1
    )

    dragging.add_object(rect)

    do -- ChatGPT 3D Box rainbow wowza
        local function hsv_to_rgb(h, s, v)
            local r, g, b

            local i = math.floor(h * 6)
            local f = h * 6 - i
            local p = v * (1 - s)
            local q = v * (1 - f * s)
            local t = v * (1 - (1 - f) * s)

            i = i % 6
            if i == 0 then r, g, b = v, t, p
            elseif i == 1 then r, g, b = q, v, p
            elseif i == 2 then r, g, b = p, v, t
            elseif i == 3 then r, g, b = p, q, v
            elseif i == 4 then r, g, b = t, p, v
            elseif i == 5 then r, g, b = v, p, q
            end

            return math.floor(r * 255), math.floor(g * 255), math.floor(b * 255)
        end

        local white = color.new(255, 0, 255, 255)

        local base_vertices = {
            {-0.5, -0.5, -0.5},
            { 0.5, -0.5, -0.5},
            { 0.5,  0.5, -0.5},
            {-0.5,  0.5, -0.5},
            {-0.5, -0.5,  0.5},
            { 0.5, -0.5,  0.5},
            { 0.5,  0.5,  0.5},
            {-0.5,  0.5,  0.5},
        }

        local indices = {
            0, 1, 1, 2, 2, 3, 3, 0,
            4, 5, 5, 6, 6, 7, 7, 4,
            0, 4, 1, 5, 2, 6, 3, 7 
        }

        local angle_y = 0
        local angle_x = 0

        local shape = renderer.write_to_buffer(
            D3D11_PRIMITIVE_TOPOLOGY.LINE_LIST,
            {},
            indices,
            nil,
            3
        )

        local time = 0

        add_connection(CONNECTIONS.PRESENT, function()
            local cosY = math.cos(angle_y)
            local sinY = math.sin(angle_y)
            local cosX = math.cos(angle_x)
            local sinX = math.sin(angle_x)

            local function rotate_xyz(v, i, time)
                local x1 = v[1] * cosY - v[3] * sinY
                local z1 = v[1] * sinY + v[3] * cosY
                local y1 = v[2]

                local y2 = y1 * cosX - z1 * sinX
                local z2 = y1 * sinX + z1 * cosX
                local x2 = x1

                local scale = 80
                local screen_x = 300 + x2 * scale
                local screen_y = 300 + y2 * scale
                local screen_z = z2 * scale

                local hue = (time + i * 0.08) % 1.0
                local r, g, b = hsv_to_rgb(hue, 1, 1)
                local c = color.new(r, g, b, 255)

                return vertex.new(screen_x, screen_y, screen_z, 1, 0, 0, c.hex)
            end

            local new_vertices = {}

            for i = 1, #base_vertices do
                table.insert(new_vertices, rotate_xyz(base_vertices[i], i, time))
            end

            shape.vertices = new_vertices

            angle_y = angle_y + 0.02
            angle_x = angle_x + 0.01

            time = time + 0.01
        end)
    end
end