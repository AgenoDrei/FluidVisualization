#version 330 core

in vec3 vUV;     //3D texture coordinates form vertex shader interpolated by rasterizer
in vec3 pos_eye;
layout(location = 0) out vec4 vFragColor;

uniform sampler3D volume;		// volume dataset
uniform float alphaFactorInc;   // adjust for overly opace values
uniform samplerCube cube_texture;
uniform mat4 view;
uniform float reflection;

const float epsilon = 0.001;

vec3 getNormal(vec3 at) {
    vec3 n = vec3(
        texture(volume, at - vec3(epsilon, 0.0, 0.0)).r - texture(volume, at + vec3(epsilon, 0.0, 0.0)).r,
        texture(volume, at - vec3(0.0, epsilon, 0.0)).r - texture(volume, at + vec3(0.0, epsilon, 0.0)).r,
        texture(volume, at - vec3(0.0, 0.0, epsilon)).r - texture(volume, at + vec3(0.0, 0.0, epsilon)).r
    );
    return normalize(n);
}

void main() {
    if (reflection!=0) {
        /* reflect ray around normal from eye to surface */
        vec3 incident_eye = normalize(pos_eye);
        vec3 normal = getNormal(vUV);       // Old staticly reflected version: vec3 normal = normalize(n_eye);

        vec3 reflected = reflect(incident_eye, normal);
        // convert from eye to world space
        reflected = vec3(inverse(view) * vec4(reflected, 0.0));

        vFragColor.rgb = texture(cube_texture, reflected).rgb;
        vFragColor.a = texture(volume, vUV).r * alphaFactorInc;

    } else {
        vFragColor = vec4(getNormal(vUV), texture(volume, vUV).r * alphaFactorInc);   // Note that the density is being read in as GL_RED value
    }
//    vFragColor = vec4(0.0f, 0.0f, 1.0f, .5f);   // show plain slices (-> Cube) ...
}
