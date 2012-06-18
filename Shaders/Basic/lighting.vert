uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;
uniform vec4 light_position;
uniform vec3 camera_position;
uniform int has_animations;
uniform mat4 bone_transforms[60];

varying vec3 normal;
varying vec3 light;
varying vec3 view;
varying float light_distance;
varying float weight_used;

void main(){
    vec3 transformed_vertex = vec3(0.0);
    weight_used = 0.0;

    if (has_animations == 1) {
        gl_TexCoord[2] = gl_MultiTexCoord2; // weights
        gl_TexCoord[3] = gl_MultiTexCoord3; // weight indices

        vec4 temp_vertex = vec4(gl_Vertex);
        vec4 temp_normal = vec4(gl_Normal, 1.0);

        // do for all four
        int weight_bone_index = int(gl_TexCoord[3][0]);
        if (weight_bone_index < 1000) {
            temp_vertex += (bone_transforms[weight_bone_index] * temp_vertex) * gl_TexCoord[2][0];
        }
        weight_bone_index = int(gl_TexCoord[3][1]);
        if (weight_bone_index < 1000) {
            weight_used = 1.0;
            temp_vertex += (bone_transforms[weight_bone_index] * temp_vertex) * gl_TexCoord[2][1];
        }
        weight_bone_index = int(gl_TexCoord[3][2]);
        if (weight_bone_index < 1000) {
            temp_vertex += (bone_transforms[weight_bone_index] * temp_vertex) * gl_TexCoord[2][2];
        }
        weight_bone_index = int(gl_TexCoord[3][3]);
        if (weight_bone_index < 1000) {
            temp_vertex += (bone_transforms[weight_bone_index] * temp_vertex) * gl_TexCoord[2][3];
        }

        transformed_vertex = (model_matrix * temp_vertex).xyz;
        normal = (normal_matrix * vec4(gl_Normal.xyz, 1.0)).xyz;
        transformed_vertex = (model_matrix * gl_Vertex).xyz;
    } else {
        normal = (normal_matrix * vec4(gl_Normal.xyz, 1.0)).xyz;
        transformed_vertex = (model_matrix * gl_Vertex).xyz;
    }

    vec3 light_vector = transformed_vertex - light_position.xyz;

    view = normalize(camera_position - transformed_vertex);

    light_distance = length(light_vector);
    light = normalize(light_vector);

    gl_Position = projection_matrix * view_matrix * vec4(transformed_vertex, 1.0);//model_matrix * gl_Vertex;
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
