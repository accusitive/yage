#ifndef YAGE_RESOURCE_HH
#define YAGE_RESOURCE_HH

#include <string>
#include <map>
#include <utility>
#include <optional>
//#include "../gen/f_triangle.glsl"
//#include "../gen/v_triangle.glsl"
#ifdef disabled
namespace yage {
    static const std::string V_TRIANGLE = "v_triangle";
    static const std::string F_TRIANGLE = "f_triangle";

    class Resource {

    public:
        std::string name;
    };

    class ShaderResource : public Resource {
    public:

        std::string source;

        ShaderResource() = default;

        explicit ShaderResource(std::string name, std::string source) {
            this->name = name;
            this->source = source;
        }

        explicit ShaderResource(const char *source) {
            this->source = std::string(source);
        }
    };

    class ShaderResourceManager {

        std::map<std::string, ShaderResource> shader_resources;
    public:
        std::optional<ShaderResource> register_shader_resource(ShaderResource res) {
            if (this->shader_resources.contains(res.name)) {
                return std::nullopt;
            } else {
                this->shader_resources[res.name] = res;
                return res;
            }
        }

        std::optional<ShaderResource> get_shader_resource(std::string name) {
            if (this->shader_resources.contains(name)) {
                return this->shader_resources.find(name)->second;

            } else {
                return std::nullopt;
            }
        }

        void register_builtin_shader_resources() {
            this->register_shader_resource(ShaderResource(V_TRIANGLE, v_triangleShaderSource[0]));
            this->register_shader_resource(ShaderResource(F_TRIANGLE, f_triangleShaderSource[0]));

        }
    };



} // yage

#endif //YAGE_RESOURCE_HH
#endif //disabled