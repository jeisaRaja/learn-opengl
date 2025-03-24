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

struct Light  {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct DirectionalLight{
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform sampler2D ourTexture;
uniform vec3 objectColor;
uniform vec3 viewPos;


uniform Material material;
uniform DirectionalLight light;


void main()
{

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
  vec3 result = ambient + diffuse + specular;
  FragColor = vec4(ambient + diffuse + specular, 1.0);
}
