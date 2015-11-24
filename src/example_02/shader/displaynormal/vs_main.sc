$input a_position, a_texcoord0, a_normal
$output v_normal

#include "common.sh"

void main()
{
	gl_Position = vec4(a_position, 1.0);
	v_normal = a_normal;
}
