$input v_normal

void main()
{
	gl_FragColor = vec4(v_normal * 0.5 + 0.5, 1.0);
}
