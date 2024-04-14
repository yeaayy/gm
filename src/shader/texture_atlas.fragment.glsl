STRINGIFY(

precision mediump float;
uniform sampler2D uTexture;
varying vec2 vUV;
void main() {
	vec4 col = texture2D(uTexture, vUV);
	if(col.a == 0.0) discard;
	gl_FragColor = col;
}

)
