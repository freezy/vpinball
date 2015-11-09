const unsigned int triggerSimpleNumVertices=49;
const unsigned int triggerSimpleNumFaces=216;
const Vertex3D_NoTex2 triggerSimple[49]=
{
{ 1.283477f,-22.522591f,33.562248f, 0.999100f,-0.036100f,0.019600f, 0.166667f,0.625000f},
{ 1.283433f,-45.411259f,-29.149363f, 0.992900f,-0.111800f,0.040700f, 0.166667f,1.000000f},
{ 0.641675f,-44.323971f,-29.149363f, 0.846900f,0.499500f,-0.182300f, 0.000000f,1.000000f},
{ 0.641719f,-21.539425f,33.071007f, 0.859200f,0.464300f,-0.214900f, 0.000000f,0.625000f},
{ 0.641719f,-21.539425f,33.071007f, 0.000000f,0.915800f,-0.401600f, 1.000000f,0.625000f},
{ 0.641675f,-44.323971f,-29.149363f, 0.000000f,0.939000f,-0.343900f, 1.000000f,1.000000f},
{ -0.641845f,-44.323971f,-29.149363f, -0.525300f,0.799000f,-0.292600f, 0.833333f,1.000000f},
{ -0.641801f,-21.539421f,33.071007f, -0.506200f,0.790100f,-0.345700f, 0.833333f,0.625000f},
{ -1.283607f,-45.411251f,-29.149363f, -0.992900f,-0.112000f,0.040600f, 0.666667f,1.000000f},
{ -1.283563f,-22.522587f,33.562248f, -0.999400f,-0.029700f,0.018800f, 0.666667f,0.625000f},
{ -0.641849f,-46.498501f,-29.149363f, -0.432100f,-0.847200f,0.309000f, 0.500000f,1.000000f},
{ -0.641805f,-23.453789f,34.053566f, -0.474400f,-0.792400f,0.383500f, 0.500000f,0.625000f},
{ 0.641671f,-46.498501f,-29.149363f, 0.432000f,-0.847300f,0.308900f, 0.333333f,1.000000f},
{ 0.641715f,-23.453793f,34.053566f, 0.476200f,-0.790200f,0.385700f, 0.333333f,0.625000f},
{ 1.283480f,-20.826500f,36.450806f, 0.992000f,-0.092900f,0.085400f, 0.166667f,0.609375f},
{ 0.641721f,-20.304619f,35.469326f, 0.856800f,0.356400f,-0.372700f, 0.000000f,0.609375f},
{ 0.641721f,-20.304619f,35.469326f, 0.000000f,0.646300f,-0.763100f, 1.000000f,0.609375f},
{ -0.641799f,-20.304615f,35.469326f, -0.559600f,0.543100f,-0.626000f, 0.833333f,0.609375f},
{ -1.283560f,-20.826496f,36.450806f, -0.992900f,-0.086600f,0.080900f, 0.666667f,0.609375f},
{ -0.641801f,-21.348299f,37.432205f, -0.454500f,-0.536000f,0.711400f, 0.500000f,0.609375f},
{ 0.641719f,-21.348303f,37.432205f, 0.456500f,-0.536500f,0.709700f, 0.333333f,0.609375f},
{ 1.283486f,-17.512125f,37.473286f, 0.993900f,0.011200f,0.109300f, 0.166667f,0.593750f},
{ 0.641726f,-17.512125f,36.364166f, 0.865200f,-0.025300f,-0.500700f, 0.000000f,0.593750f},
{ 0.641726f,-17.512125f,36.364166f, -0.000000f,-0.039700f,-0.999200f, 1.000000f,0.593750f},
{ -0.641794f,-17.512121f,36.364166f, -0.554100f,-0.038100f,-0.831600f, 0.833333f,0.593750f},
{ -1.283554f,-17.512121f,37.473286f, -0.994100f,0.011900f,0.107900f, 0.666667f,0.593750f},
{ -0.641794f,-17.512121f,38.582485f, -0.458300f,0.041900f,0.887800f, 0.500000f,0.593750f},
{ 0.641726f,-17.512125f,38.582485f, 0.458800f,0.041200f,0.887500f, 0.333333f,0.593750f},
{ 1.283604f,43.833565f,12.250843f, 0.996600f,0.049200f,0.065100f, 0.166667f,0.203125f},
{ 0.641843f,43.189926f,11.446362f, 0.851400f,-0.332400f,-0.405700f, 0.000000f,0.203125f},
{ 0.641843f,43.189926f,11.446362f, -0.000000f,-0.642100f,-0.766600f, 1.000000f,0.203125f},
{ -0.641677f,43.189926f,11.446362f, -0.530000f,-0.545800f,-0.649000f, 0.833333f,0.203125f},
{ -1.283436f,43.833572f,12.250843f, -0.996500f,0.048200f,0.068500f, 0.666667f,0.203125f},
{ -0.641674f,44.569851f,13.067883f, -0.461700f,0.561100f,0.687000f, 0.500000f,0.203125f},
{ 0.641846f,44.569851f,13.067883f, 0.461900f,0.561200f,0.686700f, 0.333333f,0.203125f},
{ 1.283607f,45.430134f,9.907923f, 0.994200f,0.104000f,0.025400f, 0.166667f,0.187500f},
{ 0.641845f,44.361649f,9.601603f, 0.865800f,-0.487900f,-0.111200f, 0.000000f,0.187500f},
{ 0.641845f,44.361649f,9.601603f, 0.000000f,-0.972700f,-0.232100f, 1.000000f,0.187500f},
{ -0.641675f,44.361649f,9.601603f, -0.548200f,-0.813100f,-0.195500f, 0.833333f,0.187500f},
{ -1.283433f,45.430141f,9.907923f, -0.994000f,0.106800f,0.023200f, 0.666667f,0.187500f},
{ -0.641671f,46.498501f,10.214362f, -0.458400f,0.860900f,0.220600f, 0.500000f,0.187500f},
{ 0.641849f,46.498501f,10.214362f, 0.458500f,0.860700f,0.221000f, 0.333333f,0.187500f},
{ 1.283600f,41.665836f,-29.149363f, 0.999500f,-0.031300f,0.003400f, 0.166667f,0.000000f},
{ 0.641838f,40.578590f,-29.149363f, 0.860100f,-0.507700f,0.049600f, 0.000000f,0.000000f},
{ 0.641838f,40.578590f,-29.149363f, 0.000000f,-0.995300f,0.097200f, 1.000000f,0.000000f},
{ -0.641682f,40.578590f,-29.149363f, -0.481100f,-0.872500f,0.084800f, 0.833333f,0.000000f},
{ -1.283440f,41.665844f,-29.149363f, -0.999500f,-0.030900f,0.003300f, 0.666667f,0.000000f},
{ -0.641678f,42.753124f,-29.149363f, -0.507500f,0.857800f,-0.081600f, 0.500000f,0.000000f},
{ 0.641842f,42.753124f,-29.149363f, 0.507300f,0.857900f,-0.082000f, 0.333333f,0.000000f}
};

const WORD triggerSimpleIndices[216]=
{
4, 5, 6,
6, 7, 4,
7, 6, 8,
16, 4, 7,
8, 9, 7,
9, 8, 10,
7, 17, 16,
17, 7, 9,
23, 16, 17,
10, 11, 9,
11, 10, 12,
9, 18, 17,
18, 9, 11,
17, 24, 23,
24, 17, 18,
30, 23, 24,
12, 13, 11,
13, 12, 1,
24, 31, 30,
37, 30, 31,
18, 25, 24,
31, 24, 25,
11, 19, 18,
25, 18, 19,
19, 11, 13,
31, 38, 37,
44, 37, 38,
38, 45, 44,
25, 32, 31,
38, 31, 32,
45, 38, 39,
32, 39, 38,
39, 46, 45,
32, 25, 26,
19, 26, 25,
39, 32, 33,
26, 33, 32,
46, 39, 40,
33, 40, 39,
40, 47, 46,
26, 19, 20,
13, 20, 19,
47, 40, 41,
41, 48, 47,
40, 33, 34,
34, 41, 40,
33, 26, 27,
27, 34, 33,
20, 27, 26,
48, 41, 35,
35, 42, 48,
36, 43, 42,
42, 35, 36,
29, 36, 35,
28, 35, 41,
35, 28, 29,
41, 34, 28,
22, 29, 28,
21, 28, 34,
28, 21, 22,
34, 27, 21,
15, 22, 21,
14, 21, 27,
21, 14, 15,
27, 20, 14,
3, 15, 14,
0, 14, 20,
14, 0, 3,
20, 13, 0,
0, 2, 3,
1, 0, 13,
0, 1, 2
};