#pragma sokol @ctype mat4 HMM_Mat4
#pragma sokol @ctype vec4 HMM_Vec4

#pragma sokol @vs sand_vs
layout(set = 0, binding = 0) uniform sand_uniforms {
    mat4 viewproj;
};

in vec2 position;
in vec2 texcoord;

out vec2 uv;

void main() {
//    gl_Position = viewproj * vec4(vec3(position.x, 1.0f - position.y, position.z), 1.0f);
    gl_Position = viewproj * vec4(position, 1.0f, 1.0f);

    uv = texcoord;
}
#pragma sokol @end

#pragma sokol @fs sand_fs
layout(set = 0, binding = 1) uniform texture2D tex;
layout(set = 0, binding = 2) uniform sampler smp;

in vec2 uv;
out vec4 frag_color;
float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    frag_color = texture(sampler2D(tex,smp), vec2(rand(uv), 1.0f - rand(uv)));
}
#pragma sokol @end
#pragma sokol @program sand sand_vs sand_fs
