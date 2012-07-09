// http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter16.html

#version 130

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 normal_matrix;

uniform vec3 camera_position;
uniform vec3 light_direction; // TODO
uniform vec3 inverse_wave_length; // TODO
uniform float camera_height; // TODO: Calculate This!
uniform float camera_height2; // TODO: Camera Height ^2
uniform float outer_radius; // Atmosphere outer radius
uniform float outer_radius2; // Atmosphere outer radius ^2
uniform float inner_radius; // Atmosphere inner radius
uniform float inner_radius2; // Atmosphere inner radius ^2
uniform float kr_esun; // Rayleigh Constant * Sun Brightness
uniform float km_esun; // Mie Constant * sun brightness
uniform float kr_4pi; // Rayleigh Constant * 4 * PI
uniform float km_4pi; // Mie Constant * 4 * PI
uniform float scale; // 1 / (outer_radius / inner_radius)
uniform scale_over_scale_depth; // scale / scale_depth

in vec4 vertex_position;
in vec4 vertex_normal;
in vec4 vertex_texcoord;
in vec4 vertex_binormal;
in vec4 vertex_bitangent;
in vec4 vertex_weights;
in ivec4 vertex_weight_indices;

smooth out vec3 color0;
smooth out vec3 color1;
smooth out vec3 texcoord0;

void main(){
    //normal = normalize((view_matrix * normal_matrix * normalize(vertex_position)).xyz);
    //gl_Position = projection_matrix * view_matrix * model_matrix * vertex_position;

    // Get the ray from the camera to the vertex and it's length
    vec3 ray = vertex_position.xyz - camera_position;
    float far = length(ray);
    ray /= far;

    // Calculate the closest intersection of the ray with the outer atmosphere
    // TODO: Write this!
    float near = getNearIntersection(camera_position, ray, camera_height2, outer_radius2);

    // calculate the ray's start and end positions in the atmosphere
    // Then calculate scattering offset
    vec3 start = camera_position + ray * near;
    far -= near;
    float start_angle = dot(ray, start) / outer_radius;
    float start_depth = exp(-inverse_scale_depth); // NOTE: Where did this come from?!?
    float start_offset = start_depth * scale(start_angle);

    // initialize the scattering loop variables
    float sample_length = far / samples; // NOTE: Where did this come from, although it's obviously something I can pick!
    float scaled_length = sample_length * scale;
    vec3 sample_ray = ray * sample_length;
    vec3 sample_point = start + sample_ray * 0.5;

    // Loop through the sample points
    vec3 front_color = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < samples; i++) {
        float height = length(sample_point);
        float depth = exp(scale_over_scale_depth * (inner_radius - height));
        float light_angle = dot(light_direction, sample_point) / height;
        float camera_angle = dot(ray, sample_point) / height;
        float scatter = (start_offset + depth * (scale(light_angle) * scale(camera_angle)));
        float attenuate = exp(-scatter * (inverse_wave_length * kr_4pi * km_4pi));
        front_color += attenuate * (depth * scaled_length);
        sample_point += sample_ray;
    }

    gl_Position = projection_matrix * view_matrix * model_matrix * vertex_position;
    color0 = front_color * (inverse_wave_length * kr_esun);
    color1 = front_color * km_esun;
    texcoord0 = camera_position - vertex_position.xyz;
}
