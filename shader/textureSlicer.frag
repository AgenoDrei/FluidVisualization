#version 330 core

smooth in vec3 vUV;     //3D texture coordinates form vertex shader interpolated by rasterizer

layout(location = 0) out vec4 vFragColor;	//fragment shader output

uniform sampler3D volume;		// volume dataset
uniform float alphaFactorInc;   // adjust for overly opace values

uniform samplerCube cube_texture;
uniform mat4 view; // view matrix

in vec3 pos_eye;
in vec3 n_eye;

void main() {
  /* reflect ray around normal from eye to surface */
  vec3 incident_eye = normalize(pos_eye);
  vec3 normal = normalize(n_eye);

  vec3 reflected = reflect(incident_eye, normal);
  // convert from eye to world space
  reflected = vec3(inverse(view) * vec4(reflected, 0.0));

  vFragColor.rgb = texture(cube_texture, reflected).rgb;
  vFragColor.a = texture(volume, vUV).r * alphaFactorInc;

    //vFragColor = vec4(0.0f, 0.0f, 1.0f, texture(volume, vUV).r * alphaFactorInc);   // Note that the density is being read in as GL_RED value
//    vFragColor = vec4(0.0f, 0.0f, 1.0f, .5f);   // show plain slices ...
}
