#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowViews[6];

out vec4 fragPos;

void main () {
	for (int i = 0; i < 6; ++i) {
		gl_Layer = i; // step through each of the faces of the shadow cube map
		for (int j = 0; j < 3; ++j) {
			fragPos = gl_in[j].gl_Position;
			gl_Position = shadowViews[i] * fragPos; // Calculate position in shadow view space
		
			EmitVertex(); // Call fragment shader for this face on this point, saves us rendering scene three times by making multiple calls to the fragment shader per point for a single render pass
		}
		EndPrimitive();
	}
}