

// Set up orthographic projection 
// this is for 640 x 960
mat4 projectionMatrix = mat4( 2.0/768.0, 0.0       ,  0.0 ,  0.0,
                              0.0      ,-2.0/768.0 ,  0.0 ,  0.0,
                              0.0      , 0.0       ,  1.0 ,  0.0,
                             -1.0      , 1.0       ,  0.0 ,  1.0);                        

void main()
{
    gl_FrontColor = gl_Color;
    gl_Position = projectionMatrix * gl_Vertex;
}