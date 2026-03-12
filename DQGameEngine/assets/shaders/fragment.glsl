#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
    int index = int(v_TexIndex);

    //color = vec4(1,0,0,1); 
    color = texture(u_Textures[index], v_TexCoord);
}

