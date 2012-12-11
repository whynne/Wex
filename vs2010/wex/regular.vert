uniform mat4 vs_Projection;
uniform mat4 vs_ModelView;
                           
attribute vec4 vs_Vertex;                                    
attribute vec2 vs_TexCoord;
attribute vec4 vs_Color;
                                                               
varying vec2 fs_TexCoord;
varying vec4 fs_Color;
void main()
{
    gl_Position  = vs_Projection * vs_ModelView * vs_Vertex;
    fs_TexCoord = vs_TexCoord;
    fs_Color    = vs_Color;
}