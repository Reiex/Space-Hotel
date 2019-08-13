uniform sampler2D textureCourrante;
uniform int nombreDeFlags;
uniform float flags[];
uniform float temperature;

vec4 getCouleurPixel()
{
	return texture2D(textureCourrante, gl_TexCoord[0].xy);
}

void main()
{
	vec4 couleur = getCouleurPixel();
	
	// Creation
	
	if (flags[0] == 1.0)
	{
		couleur.a *= 0.5;
	}
	
	// Selection
	
	if (flags[1] == 1.0)
	{
		couleur.r = 0.0;
		couleur.b = 0.0;
	}
	
	// MultiSelection
	
	if (flags[2] == 1.0)
	{
		couleur.b = 0.0;
	}
	
	// Panne
	
	if (flags[3] == 1.0)
	{
		couleur.r *= 0.2;
		couleur.g *= 0.2;
		couleur.b *= 0.2;
	}
	
	if (temperature > 30.0)
	{
		couleur.r *= 1.0 + 0.1 * exp((temperature - 30.0)*0.2);
	}
	
	// Affichage
	
	gl_FragColor = couleur;
}