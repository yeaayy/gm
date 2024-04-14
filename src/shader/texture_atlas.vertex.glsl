STRINGIFY(

attribute vec4 aPosition;
attribute vec2 aUV;
uniform vec4 uPosition;
uniform mat4 uMatrix;
uniform mat4 uMatrix2;
varying vec2 vUV;
void main(){
   vUV = aUV;
   gl_Position = uMatrix2 * (aPosition +  uPosition);
}

)
