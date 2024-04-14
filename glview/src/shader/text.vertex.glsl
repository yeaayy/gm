STRINGIFY(
attribute vec4 aPosition;
attribute vec2 aUV;
uniform vec2 uScale;
uniform vec2 uPosition;
varying vec2 vUV;

void main() {
   vUV = aUV;
//   gl_Position = vec4(uScale * aPosition + uPosition, 0, 1);
   gl_Position = aPosition;
}
)