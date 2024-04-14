STRINGIFY(

precision mediump float;

uniform sampler2D uTexture;
uniform vec4 uColor;
varying vec2 vUV;

void main() {
	vec4 color = texture2D(uTexture, vUV);
	if(color.a == 0.0) discard;
    gl_FragColor = color * uColor;
}

)