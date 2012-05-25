uniform sampler2D decal_map;
uniform sampler2D height_map; // ignore
uniform vec4 material_color;

void main(){
    gl_FragColor = texture2D(decal_map, gl_TexCoord[0].st) * material_color;
}
