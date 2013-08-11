uniform sampler2D fs_Sampler0;

varying vec2 fs_TexCoord;
varying vec4 fs_Color;

void main() 

{
      gl_FragColor = texture2D(fs_Sampler0, fs_TexCoord.st) * fs_Color;
} 