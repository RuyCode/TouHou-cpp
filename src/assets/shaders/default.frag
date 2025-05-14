// shader.frag
uniform sampler2D frame;

void main()
{
   gl_FragColor = texture2D(frame, gl_TexCoord[0].xy);
}
