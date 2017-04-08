#version 330 core
out vec4 vFragColor;	//fragment shader output

in vec3 vUV;				//3D texture coordinates form vertex shader interpolated by rasterizer

uniform sampler3D	volume;		//volume dataset
uniform samplerCube cubeTexture;
uniform vec3		camPos;		//camera position
uniform vec3		step_size;	//ray step size
uniform vec3        lightDir;   //directional light

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

vec3 getShadow(vec3 at) {
    vec3 dataPos = at;
    vec3 geomDir = -lightDir;
    vec3 dirStep = geomDir * step_size * 5;
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
	//get the 3D texture coordinates for lookup into the volume dataset
	vec3 dataPos = vUV;
	vec3 normal = vec3(1.0f);
	vFragColor.rgba = vec4(0.0f);

	//Getting the ray marching direction:
	//get the object space position by subracting 0.5 from the
	//3D texture coordinates. Then subtraact it from camera position
	//and normalize to get the ray marching direction
	vec3 geomDir = normalize(vUV - camPos);
	//vec3 geomDir = normalize(vUV - camPos);

	//multiply the raymarching direction with the step size to get the
	//sub-step size we need to take at each raymarching step
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
		    vec3 reflected = reflect(geomDir, normal);
		    vec3 shadow = getShadow(dataPos);
            vFragColor.rgb = texture(cubeTexture, reflected).rgb * vec3(.5, .5, .6) * shadow;
		}

		//Opacity calculation using compositing:
		//here we use front to back compositing scheme whereby the current sample
		//value is multiplied to the currently accumulated alpha and then this product
		//is subtracted from the sample value to get the alpha from the previous steps.
		//Next, this alpha is multiplied with the current sample colour and accumulated
		//to the composited colour. The alpha value from the previous steps is then
		//accumulated to the composited colour alpha.
		float prev_alpha = sampleValue - (sampleValue * vFragColor.a);
		//vFragColor.rgb = prev_alpha * sampleValue + vFragColor.rgb;
		vFragColor.a += prev_alpha;

		//early ray termination
		//if the currently composited colour alpha is already fully saturated
		//we terminated the loop
		if( vFragColor.a>0.8) {
		    vFragColor.a = 0.8;
			break;
		}
	}
}
