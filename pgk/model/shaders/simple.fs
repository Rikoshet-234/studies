#version 120

uniform mat4 view;
uniform sampler2D sampler;

uniform float textureMode;
uniform float lightsOn;
uniform float alertPhase;

varying vec3 fragmentPosition;
varying vec2 fragmentUV;
varying vec3 fragmentNormal;

varying vec3 eyeDirection;


struct lightSource {
	vec3 position;
	vec3 color;
	float power;
};


struct materialProperties {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


float snoise(vec3 v); // Defined in `perlin.glsl`


vec3 phong(lightSource light, materialProperties material) {
	vec3 direction = (view * vec4(light.position, 1.0)).xyz + eyeDirection;

	float distance = length(light.position - fragmentPosition);
	float fade = distance * distance;

	// Ambient light calculation.
	vec3 ambient = material.ambient * light.color;

	// Diffuse light calculation.
	vec3 N = normalize(fragmentNormal);
	vec3 L = normalize(direction);
	float theta = clamp(dot(N, L), 0.0, 1.0);
	vec3 diffuse = material.diffuse * light.power * light.color * theta / fade;

	// Specular light calculation.
	vec3 E = normalize(eyeDirection);
	vec3 R = reflect(-L, N);
	float alpha = clamp(dot(E, R), 0.0, 1.0);
	vec3 specular = material.specular * light.power * light.color * pow(alpha, 5.0) / fade;

	return ambient + diffuse + specular;
}


void main() {
	lightSource staticLight = lightSource(
		vec3(3.0f, -3.0f, 3.0f),
		vec3(1.0, 1.0, 1.0),
		5.0
	);

	lightSource alarmLight = lightSource(
		vec3(-5.0, +5.0, 0.0),
		vec3(1.0, 0.0, 0.0),
		7.5 * alertPhase
	);

	materialProperties material = materialProperties(
		vec3(0.01, 0.01, 0.01),
		vec3(0.0),
		vec3(0.7, 0.7, 0.7)		
	);

	if (textureMode >= 0.40f) {
		material.diffuse = vec3(snoise(fragmentUV.xyx * 50) + 0.5);
	} else if (textureMode >= 0.20f) {
		material.diffuse = texture2D(sampler, fragmentUV).rgb;
	} else {
		material.diffuse = vec3(1.0, 1.0, 1.0);
	}


	if (lightsOn > 0.5) {
		gl_FragColor = vec4(
			phong(staticLight, material) + phong(alarmLight, material),
			1.0
		);
	} else {
		gl_FragColor = vec4(material.diffuse, 1.0);
	}
}
