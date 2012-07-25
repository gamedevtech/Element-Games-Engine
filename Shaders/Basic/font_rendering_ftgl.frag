uniform sampler2D decal_map;
uniform vec4 material_color;

void main(){
    vec4 decal_color = texture2D(decal_map, gl_TexCoord[0].st);
    decal_color = vec4(1.0 - decal_color.r, 1.0 - decal_color.g, 1.0 - decal_color.b, decal_color.a);
    gl_FragColor = decal_color * material_color;
}
