#version 330
@ctype mat4 hmm_mat4
@ctype vec4 hmm_vec4

@vs vs
uniform vs_per_frame {
    mat4 viewproj;
};

layout(location=0) in vec4 position;
layout(location=1) in vec4 color0;
out vec4 color;
void main() {
    gl_Position = viewproj * position;
    color = color0;
}