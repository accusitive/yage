# https://gist.github.com/christophercrouzet/67de1b021e0c471c7c3b
SET(FILES "v_triangle.glsl" "f_triangle.glsl")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../assets/shaders")
set(DESTINATION_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../engine/gen")

separate_arguments(FILES)

foreach (file ${FILES})
    if (LOAD_SHADERS_DYNAMICALLY)
        file(
                COPY ${SOURCE_DIR}/${file}
                DESTINATION ${DESTINATION_DIR}
        )
    else()
        set(filename ${SOURCE_DIR}/${file})
        get_filename_component(name ${filename} NAME_WE)
        get_filename_component(extension ${filename} EXT)

        set(source "")

        file(STRINGS ${filename} lines)
        foreach (line ${lines})
            set(source "${source}    \"${line}\\n\"\n")
        endforeach()

        file(
                WRITE ${DESTINATION_DIR}/${file}
                "// Auto generated by engine/cmake/shaders.cmake\n"
                "static const char *${name}ShaderSource[] = {\n"
                "${source}"
                "};\n"
        )
    endif()
endforeach()