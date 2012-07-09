// http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter16.html

#version 130

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 normal_matrix;

uniform vec3 camera_position;

const float kr_esun = 0.05; // esun = 20.0f (esun should be sent in)
const float km_esun = 0.02; // esun = 20.0f (esun should be sent in)
const int sample_count = 2;
const float samples = 2.0;
const float kr_4pi = 0.0314159265; // Rayleigh Constant * 4 * PI
const float km_4pi = 0.0125663706; // Mie Constant * 4 * PI

in vec4 vertex_position;
in vec4 vertex_normal;
in vec4 vertex_texcoord;
in vec4 vertex_binormal;
in vec4 vertex_bitangent;
in vec4 vertex_weights;
in ivec4 vertex_weight_indices;

smooth out vec3 color0;
smooth out vec3 color1;
smooth out vec3 direction;

float scale(float cosine_value, float scale_depth) {
    float x = 1.0 - cosine_value;
    return scale_depth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

void main(){
    vec3 light_position = vec3(0.0, 0.0, 0.0); // NOTE: Test Data
    float inner_radius = 1.0;
    float outer_radius = 1.3;

    float camera_height = 0.0; // TODO
    float camera_height2 = camera_height * camera_height;
    float outer_radius2 = outer_radius * outer_radius;
    float inner_radius2 = inner_radius * inner_radius;
    float scale_value = 1 / (outer_radius / inner_radius);
    float scale_depth = 0.25;
    float scale_over_scale_depth = scale_value / scale_depth;
    vec3 inverse_wave_length = vec3(
        1.0 / pow(0.650, 4.0),
        1.0 / pow(0.570, 4.0),
        1.0 / pow(0.475, 4.0)
    );
    int camera_inside_atmosphere = 0;

    // Get the ray from the camera to the vertex and it's length
    vec3 ray = vertex_position.xyz - camera_position;
    float far = length(ray);
    ray /= far;

    // Calculate the closest intersection of the ray with the outer atmosphere
    float start_offset = 0.0;
    vec3 start = vec3(0.0, 0.0, 0.0);
    if (camera_inside_atmosphere == 0) {
        // Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
        float B = 2.0 * dot(camera_position, ray);
        float C = camera_height2 - outer_radius2;
        float det = max(0.0, B*B - 4.0 * C);
        float near = 0.5 * (-B - sqrt(det));

        // Calculate the ray's starting position, then calculate its scattering offset
        start = camera_position + ray * near;
        far -= near;
        float start_angle = dot(ray, start) / outer_radius;
        float start_depth = exp(-1.0 / scale_depth);
        start_offset = start_depth * scale(start_angle, scale_depth);
    } else {
        start = camera_position;
        float height = length(start);
        float depth = exp(scale_over_scale_depth * (inner_radius - camera_height));
        float start_angle = dot(ray, start) / height;
        start_offset = depth * scale(start_angle, scale_depth);
    }

    // initialize the scattering loop variables
    float sample_length = far / samples; // NOTE: Where did this come from, although it's obviously something I can pick!
    float scaled_length = sample_length * scale_value;
    vec3 sample_ray = ray * sample_length;
    vec3 sample_point = start + sample_ray * 0.5;

    // Loop through the sample points
    vec3 front_color = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < sample_count; i++) {
        float height = length(sample_point);
        float depth = exp(scale_over_scale_depth * (inner_radius - height));
        float light_angle = dot(light_position, sample_point) / height;
        float camera_angle = dot(ray, sample_point) / height;
        float scatter = (start_offset + depth * (scale(light_angle, scale_depth) * scale(camera_angle, scale_depth)));
        vec3 attenuate = exp(-scatter * (inverse_wave_length * kr_4pi * km_4pi));
        front_color += attenuate * (depth * scaled_length);
        sample_point += sample_ray;
    }

    gl_Position = projection_matrix * view_matrix * model_matrix * vertex_position;
    color0 = front_color * (inverse_wave_length * kr_esun);
    color1 = front_color * km_esun;
    direction = camera_position - vertex_position.xyz;
}
