uniform sampler2D decal;
uniform vec4 color;

void main(){
    vec4 decal_color = texture2D(decal, gl_TexCoord[0].st);
    decal_color = vec4(1.0 - decal_color.r, 1.0 - decal_color.g, 1.0 - decal_color.b, decal_color.a);
    gl_FragColor = decal_color * color;
}
