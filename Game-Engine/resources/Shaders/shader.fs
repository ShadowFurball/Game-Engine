#version 430

in vec3 vertPos; //Vertex position in eye coords
in vec3 N; //Transformed normal
in vec3 lightPos; //Light position in eye coords

layout (location = 0) out vec4 FragColour;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ld;            // Diffuse light intensity

uniform vec3 Ia;			//Ambient light intensity
uniform vec3 Ka;			//Ambient reflectivity

uniform vec3 Ep;			//Eye Position
uniform vec3 Ks;			//Specular refelctivity
uniform int n;				//Specular intensity

void main() 
{
	//Ambient Light
	vec4 ambient = vec4(Ld * Ka, 1.0);

	//Diffuse
	vec3 L = normalize(lightPos - vertPos); 
	vec4 diffuse = vec4(Ld,1.0) * max(dot(N,L), 0.0);
	vec4 diff = vec4(Kd,1.0) * diffuse;
	diffuse = clamp(diffuse, 0.0, 1.0);

	//Specular Lighting
	vec3 R = reflect(-L, N);
	vec3 view = normalize(Ep - vertPos);
	vec4 specular = diffuse * vec4(Ks, 1.0) * pow(max(0,dot(R, view)), n);
	
	//FragColour = ambient;
	//FragColour = diff;
	//FragColour = diff + specular;
	FragColour = ambient + diff + specular;

} 
