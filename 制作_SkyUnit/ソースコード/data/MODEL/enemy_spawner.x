xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 19;
 0.00000;32.44827;0.00000;,
 -16.22414;-0.00000;-28.10104;,
 -32.44827;-0.00000;0.00000;,
 0.00000;32.44827;0.00000;,
 16.22414;-0.00000;-28.10104;,
 0.00000;32.44827;0.00000;,
 32.44827;-0.00000;0.00000;,
 0.00000;32.44827;0.00000;,
 16.22414;-0.00000;28.10104;,
 0.00000;32.44827;0.00000;,
 -16.22413;-0.00000;28.10104;,
 0.00000;32.44827;0.00000;,
 -32.44827;-0.00000;0.00000;,
 0.00000;-32.44827;-0.00000;,
 0.00000;-32.44827;-0.00000;,
 0.00000;-32.44827;-0.00000;,
 0.00000;-32.44827;-0.00000;,
 0.00000;-32.44827;-0.00000;,
 0.00000;-32.44827;-0.00000;;
 
 12;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;2,1,13;,
 3;1,4,14;,
 3;4,6,15;,
 3;6,8,16;,
 3;8,10,17;,
 3;10,12,18;;
 
 MeshMaterialList {
  4;
  12;
  0,
  3,
  0,
  0,
  0,
  0,
  2,
  1,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.370196;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.062745;0.062745;0.062745;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.065882;0.800000;0.153725;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  12;
  -0.654654;0.654654;-0.377964;,
  0.000000;0.654654;-0.755929;,
  0.654654;0.654654;-0.377964;,
  0.654654;0.654654;0.377964;,
  0.000000;0.654654;0.755929;,
  -0.654654;-0.654654;-0.377964;,
  -0.654654;0.654654;0.377964;,
  0.000000;-0.654654;-0.755929;,
  0.654654;-0.654654;-0.377964;,
  0.654654;-0.654654;0.377964;,
  0.000000;-0.654654;0.755929;,
  -0.654654;-0.654654;0.377964;;
  12;
  3;0,0,0;,
  3;1,1,1;,
  3;2,2,2;,
  3;3,3,3;,
  3;4,4,4;,
  3;6,6,6;,
  3;5,5,5;,
  3;7,7,7;,
  3;8,8,8;,
  3;9,9,9;,
  3;10,10,10;,
  3;11,11,11;;
 }
 MeshTextureCoords {
  19;
  0.083333;0.000000;,
  0.166667;0.500000;,
  0.000000;0.500000;,
  0.250000;0.000000;,
  0.333333;0.500000;,
  0.416667;0.000000;,
  0.500000;0.500000;,
  0.583333;0.000000;,
  0.666667;0.500000;,
  0.750000;0.000000;,
  0.833333;0.500000;,
  0.916667;0.000000;,
  1.000000;0.500000;,
  0.083333;1.000000;,
  0.250000;1.000000;,
  0.416667;1.000000;,
  0.583333;1.000000;,
  0.750000;1.000000;,
  0.916667;1.000000;;
 }
}
