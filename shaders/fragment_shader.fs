#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material  {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct PointLight  {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct DirectionalLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct SpotLight {
  vec3 position;
  vec3 direction;

  float cutoff;
  float outerCutoff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform sampler2D ourTexture;
uniform vec3 objectColor;
uniform vec3 viewPos;


uniform Material material;
uniform SpotLight light;


void main()
{
  float distance = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);

  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff)/ epsilon, 0.0, 1.0);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
  
   ambient *= attenuation;
   diffuse *= intensity;
   specular *= intensity;

  vec3 result = ambient + diffuse + specular;
  FragColor = vec4(ambient + diffuse + specular, 1.0);
}
