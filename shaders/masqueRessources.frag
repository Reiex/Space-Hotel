uniform sampler2D textureCourrante;
uniform sampler2D textureMasque;

vec4 getCouleurPixel()
{
	return texture2D(textureCourrante, gl_TexCoord[0].xy);
}

vec4 getCouleurMasque()
{
	return texture2D(textureMasque, gl_TexCoord[0].xy);
}

void main()
{
	vec4 couleur = getCouleurPixel();
	vec4 masque = getCouleurMasque();
	
	couleur.a = masque.a;
	
	gl_FragColor = couleur;
}