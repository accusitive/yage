@ctype mat4 HMM_Mat4
@ctype vec4 HMM_Vec4

@vs vs
layout(set = 0, binding = 0) uniform vs_per_frame {
    mat4 viewproj;
};

in vec4 position;
in vec4 color0;

out vec4 color;

void main() {
    gl_Position = viewproj * position;
    color = color0;
}
@end

@fs fs
in vec4 color;
out vec4 frag_color;
void main() {
    frag_color = color;
}
@end
@program triangle vs fs