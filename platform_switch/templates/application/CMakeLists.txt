## Targets
set(CMAKE_CXX_STANDARD 20)

# First, we want to add the regular ELF target.
add_executable(yage_app source/switch_main.cc)

# Next, we need to set the required target properties
# so CMake can detect the metadata for the application.
set_target_properties(yage_app PROPERTIES
        APP_TITLE "Template Application"
        APP_AUTHOR "Valentin B."
        APP_VERSION "0.1.0")

# Then, we link the required libraries (libnx) against the executable.
#find_package(OpenGL REQUIRED)
#find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL GLX)
target_include_directories(yage_app PRIVATE ../../../engine /opt/devkitpro/portlibs/switch/include)
target_link_libraries(yage_app PRIVATE
       yage-engine
        ${DEVKITPRO}/portlibs/switch/lib/libEGL.a
        ${DEVKITPRO}/portlibs/switch/lib/libglapi.a
        ${DEVKITPRO}/portlibs/switch/lib/libglad.a
        ${DEVKITPRO}/portlibs/switch/lib/libdrm_nouveau.a

        switch::libnx
)

#target_link_libraries(my_app
#        PRIVATE
#        yage-engine
#)
# Set these variables if you don't want to use an icon or
# don't want `add_nro_target` to generate a .nacp file.
set(NO_ICON TRUE)
set(NO_NACP TRUE)

# And finally we can add the actual binary (.nro) target.
add_nro_target(yage_app)
