#version 330 core

in vec2 texPos;

out float fragColour;

uniform sampler2D ssao;
uniform bool blurEnable;

void main() {

	if (blurEnable) {
		vec2 texelSize = 1.0f / vec2(textureSize(ssao, 0));
		float result = 0.0f;
		for (int x = -2; x < 2; ++x) {
			for (int y = -2; y < 2; ++y) { 
				vec2 offset = vec2(float(x), float(y)) * texelSize;
				result += texture(ssao, texPos + offset).r;
			}
		}
		fragColour = result / 16.0f;
	}
	else fragColour = texture(ssao, texPos).r;
}