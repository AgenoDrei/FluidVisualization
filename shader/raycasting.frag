#version 330 core
out vec4 vFragColor;	//fragment shader output

in vec3 vUV;				//3D texture coordinates form vertex shader interpolated by rasterizer

uniform sampler3D	volume;		//volume dataset
uniform samplerCube cubeTexture;
uniform sampler2D   randomValues;
uniform vec3		camPos;		//camera position
uniform vec3		step_size;	//ray step size
uniform vec3        lightPos;   //directional light
uniform float       reflection;
uniform float       shadow;

//constants
const int MAX_SAMPLES = 300;	//total samples for each ray march step
const vec3 texMin = vec3(0.0);	//minimum texture access coordinate
const vec3 texMax = vec3(1.0);	//maximum texture access coordinate
const float normalVoxelSize = 0.001;
const vec3 ambient = vec3(0.5, 0.5, 0.6);


vec3 getNormal(vec3 at) {
    vec3 n = vec3(texture(volume, at - vec3(normalVoxelSize, 0.0, 0.0)).a - texture(volume, at + vec3(normalVoxelSize, 0.0, 0.0)).a,
        texture(volume, at - vec3(0.0, normalVoxelSize, 0.0)).a - texture(volume, at + vec3(0.0, normalVoxelSize, 0.0)).a,
        texture(volume, at - vec3(0.0, 0.0, normalVoxelSize)).a - texture(volume, at + vec3(0.0, 0.0, normalVoxelSize)).a);

    return normalize(n);
}

vec3 getReflection(vec3 dir, vec3 n) {
    if(reflection != 1.0f)
        return vec3(1.0f);

    vec3 reflected = reflect(dir, n);
    return texture(cubeTexture, reflected).rgb;
}

vec3 getShadow(vec3 at) {
    if(shadow != 1.0f)
        return vec3(1.0f);

    vec3 dataPos = at;
    vec3 geomDir = normalize(lightPos - dataPos);
    vec3 dirStep = geomDir * step_size * 2.0f;
    bool shadow = false;

    for (int i = 0; i < MAX_SAMPLES; i++) {
        dataPos = dataPos + dirStep;
        if(dataPos.x > texMax.x || dataPos.x < texMin.x
        		    || dataPos.y > texMax.y || dataPos.y < texMin.y
        		    || dataPos.z > texMax.z || dataPos.z < texMin.z) {
        		    return vec3(1.0f);
        }
        float sampleValue = texture(volume, dataPos).a;
        if(sampleValue != 0.0f)
            return vec3(0.5f);
    }

    return vec3(1.0f);
}

void main()
{
    vec3 randomFactor = vec3(texture(randomValues, vec2(vUV.x, vUV.y)).r);
	//get the 3D texture coordinates for lookup into the volume dataset
	vec3 dataPos = vUV + randomFactor;
	vec3 normal = vec3(1.0f);
	vFragColor.rgba = vec4(0.0f);

	//Getting the ray marching direction:
	vec3 geomDir = normalize(vUV - camPos);
	//vec3 geomDir = normalize(vUV - camPos);

	//multiply the raymarching direction with the step size to get the sub-step size we need to take at each raymarching step
	vec3 dirStep = geomDir * step_size;

	//flag to indicate if the raymarch loop should terminate
	bool fluidEntered = false;

	//for all samples along the ray
	for (int i = 0; i < MAX_SAMPLES; i++) {
		// advance ray by dirstep
		dataPos = dataPos + dirStep;

		if(dataPos.x > texMax.x || dataPos.x < texMin.x
		    || dataPos.y > texMax.y || dataPos.y < texMin.y
		    || dataPos.z > texMax.z || dataPos.z < texMin.z) {
		    break;
		}
        /*stop = dot(sign(dataPos-texMin),sign(texMax-dataPos)) < 3.0;
		if (stop)
			break;*/

		// data fetching from the alpha channel of volume texture
		float sampleValue = texture(volume, dataPos).a;

        //Enter Fluid
		if(sampleValue != 0.0f && !fluidEntered) {
		    fluidEntered = true;
		    vec3 normal = getNormal(dataPos);
		    vec3 reflections = getReflection(geomDir, normal);
		    vec3 shadows = getShadow(dataPos);
		    if(reflection != 1.0f && shadow != 1.0f)
		        vFragColor.rgb = normal;
		    else
                vFragColor.rgb = reflections * vec3(.5, .5, .6) * shadows;
		}

		//Opacity calculation using front to back compositing:
		float prev_alpha = sampleValue - (sampleValue * vFragColor.a);
		vFragColor.a += prev_alpha;

		//early ray termination
		if( vFragColor.a>0.8) {
		    vFragColor.a = 0.8;
			break;
		}
	}
}
