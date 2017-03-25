#version 330 core

smooth in vec3 vUV;     //3D texture coordinates form vertex shader interpolated by rasterizer

layout(location = 0) out vec4 vFragColor;	//fragment shader output

uniform sampler3D volume;		// volume dataset
uniform float alphaFactorInc;   // adjust for overly opace values

void main() {
    vFragColor = vec4(0.0f, 0.0f, 1.0f, texture(volume, vUV).r * alphaFactorInc);   // Note that the density is being read in as GL_RED value
//    vFragColor = vec4(0.0f, 0.0f, 1.0f, .5f);   // show plain slices ...
}
