uniform sampler2D decal_map;
uniform sampler2D height_map; // ignore
uniform vec4 material_color;

varying vec3 normal;

void main(){
    float factor = 1.0 - dot(normalize(normal), normalize(vec3(0.0, 0.0, -1.0))) + 0.1;
    gl_FragColor = vec4(material_color.r, material_color.g, material_color.b, factor);
}
