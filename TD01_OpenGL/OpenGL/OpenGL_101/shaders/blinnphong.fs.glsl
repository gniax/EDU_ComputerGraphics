#version 120

// variable predefinie 
// en sortie: gl_FragColor
// elle est de type vec4

uniform float u_Time; // maj par glUniform1f

uniform sampler2D u_Sampler;

varying vec2 v_TexCoords;
varying vec3 v_Normal;

varying vec3 v_ViewDirection;

uniform vec3 u_LightDirection; // direction VERS la lumiere

// materiau
struct Material
{
    vec3 DiffuseColor;
    vec3 SpecularColor;
    float Shininess;
};
uniform Material u_Material;

float diffuse(vec3 N, vec3 L)
{
    float lambert = max(dot(N, L), 0.0);
    return lambert;
}

float blinn_phong(vec3 N, vec3 L, vec3 V, float shininess)
{
    // la reflexion speculaire ne s'affiche que 
    // sur les faces orientees vers la source lumineuse
    float visible = dot(N, L) > 0.0 ? 1.0 : 0.0;

    vec3 H = normalize(L + V);
    float blinn = max(dot(N, H), 0.0);
    blinn = visible * pow(blinn, shininess);
    return blinn;
}

void main(void)
{
    // il est tres important de renormaliser les normales
    // interpolees depuis le Vertex Shader
    vec3 N = normalize(v_Normal);
    vec3 V = normalize(v_ViewDirection);
    vec3 L = normalize(u_LightDirection); // pas utile en pratique ici

    vec4 texColor = texture2D(u_Sampler, v_TexCoords);
    vec3 diffuseColor = diffuse(N, L) * u_Material.DiffuseColor * texColor.rgb;

    // la valeur de shininess de Blinn par rapport a celle de Phong
    // est en moyenne 2 a 4 fois plus faible:
    float blinn_shininess = 4.0 * u_Material.Shininess;
    vec3 specularColor = blinn_phong(N, L, V, blinn_shininess)
                        * u_Material.SpecularColor;

    vec3 finalColor = diffuseColor + specularColor;

    gl_FragColor = vec4(finalColor, texColor.a);
}