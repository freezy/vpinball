const unsigned int triggerStarNumVertices=231;
const unsigned int triggerStarNumFaces=510;
Vertex3D_NoTex2 triggerStar[231]=
{
{ 0.000000f,0.002343f,0.291799f, 0.000000f,0.000000f,1.000000f, 0.500000f,0.430000f},
{ 0.179117f,-0.055860f,0.291800f, 0.061500f,-0.020000f,0.997900f, 0.524350f,0.504941f},
{ 0.179117f,0.060547f,0.291799f, 0.061500f,0.020000f,0.997900f, 0.475650f,0.504941f},
{ 0.467958f,-0.043648f,0.255800f, 0.179400f,0.000000f,0.983800f, 0.519241f,0.625788f},
{ 0.467958f,0.048334f,0.255799f, 0.179400f,0.000000f,0.983800f, 0.480759f,0.625788f},
{ 0.179117f,0.060546f,-0.036547f, 0.951100f,0.309000f,0.000200f, 0.185085f,0.998195f},
{ 0.179117f,0.060547f,0.291799f, 0.951100f,0.309000f,-0.000000f, 0.185085f,0.867195f},
{ 0.467958f,0.048334f,0.255799f, 0.042900f,0.999100f,-0.000600f, 0.295842f,0.875267f},
{ 0.179117f,-0.055860f,0.291800f, 0.951100f,-0.309000f,0.000000f, 0.814285f,0.867195f},
{ 0.179117f,-0.055861f,-0.036547f, 0.951100f,-0.309000f,0.000200f, 0.814285f,0.998195f},
{ 0.467958f,-0.043648f,0.255800f, 0.042900f,-0.999100f,-0.000600f, 0.702373f,0.875267f},
{ 0.968000f,-0.022493f,0.104600f, 0.042100f,-0.999100f,0.001000f, 0.509685f,0.901977f},
{ 0.791931f,-0.029583f,0.176600f, 0.041900f,-0.999100f,0.002600f, 0.578849f,0.888510f},
{ 0.791931f,0.034269f,0.176599f, 0.307100f,0.000000f,0.951700f, 0.486643f,0.761335f},
{ 0.791931f,-0.029583f,0.176600f, 0.307100f,0.000000f,0.951700f, 0.513357f,0.761335f},
{ 0.791931f,0.034269f,0.176599f, 0.041900f,0.999100f,0.002600f, 0.420115f,0.888510f},
{ 0.968000f,0.027179f,0.104599f, 0.378500f,0.000000f,0.925600f, 0.489610f,0.835000f},
{ 0.968000f,-0.022493f,0.104600f, 0.378500f,0.000000f,0.925600f, 0.510390f,0.835000f},
{ 0.968000f,0.027179f,0.104599f, 0.042100f,0.999100f,0.001000f, 0.489685f,0.901977f},
{ 0.968000f,0.027179f,0.104599f, 1.000000f,-0.000000f,-0.000000f, 0.489685f,0.901977f},
{ 0.968000f,-0.022493f,0.104600f, 1.000000f,-0.000000f,-0.000000f, 0.509685f,0.901977f},
{ 0.968000f,0.027178f,-0.036547f, 1.000000f,-0.000000f,-0.000000f, 0.489685f,0.998195f},
{ 0.968000f,-0.022494f,-0.036547f, 1.000000f,-0.000000f,-0.000000f, 0.509685f,0.998195f},
{ 0.968000f,-0.022494f,-0.036547f, 0.042300f,-0.999100f,0.000000f, 0.509685f,0.998195f},
{ 0.968000f,0.027178f,-0.036547f, 0.042300f,0.999100f,-0.000000f, 0.489685f,0.998195f},
{ 0.110700f,-0.150035f,0.291800f, 0.038000f,-0.052300f,0.997900f, 0.563748f,0.476316f},
{ 0.351555f,-0.309946f,0.255800f, 0.145100f,-0.105400f,0.983800f, 0.630647f,0.577087f},
{ 0.405617f,-0.235531f,0.255800f, 0.145100f,-0.105400f,0.983800f, 0.599516f,0.599705f},
{ 0.179117f,-0.055861f,-0.036547f, 0.951100f,-0.309000f,0.000200f, 0.185085f,0.998195f},
{ 0.179117f,-0.055860f,0.291800f, 0.951100f,-0.309000f,0.000000f, 0.185085f,0.867195f},
{ 0.405617f,-0.235531f,0.255800f, 0.622000f,0.783000f,-0.000600f, 0.295842f,0.875267f},
{ 0.110700f,-0.150035f,0.291800f, 0.587800f,-0.809000f,0.000000f, 0.814285f,0.867195f},
{ 0.110700f,-0.150036f,-0.036547f, 0.587800f,-0.809000f,0.000200f, 0.814285f,0.998195f},
{ 0.351555f,-0.309946f,0.255800f, -0.552600f,-0.833500f,-0.000600f, 0.702373f,0.875267f},
{ 0.768531f,-0.586773f,0.104601f, -0.553300f,-0.833000f,0.001000f, 0.509685f,0.901977f},
{ 0.621922f,-0.489010f,0.176600f, -0.553400f,-0.832900f,0.002600f, 0.578849f,0.888510f},
{ 0.659450f,-0.437352f,0.176600f, 0.248400f,-0.180500f,0.951700f, 0.683948f,0.705906f},
{ 0.621922f,-0.489010f,0.176600f, 0.248400f,-0.180500f,0.951700f, 0.705559f,0.690205f},
{ 0.659450f,-0.437352f,0.176600f, 0.621200f,0.783600f,0.002600f, 0.420115f,0.888510f},
{ 0.797726f,-0.546587f,0.104601f, 0.306200f,-0.222500f,0.925600f, 0.729647f,0.763759f},
{ 0.768531f,-0.586773f,0.104601f, 0.306200f,-0.222500f,0.925600f, 0.746459f,0.751545f},
{ 0.797726f,-0.546587f,0.104601f, 0.621300f,0.783500f,0.001000f, 0.489685f,0.901977f},
{ 0.797726f,-0.546587f,0.104601f, 0.809000f,-0.587800f,0.000000f, 0.489685f,0.901977f},
{ 0.768531f,-0.586773f,0.104601f, 0.809000f,-0.587800f,0.000000f, 0.509685f,0.901977f},
{ 0.797726f,-0.546588f,-0.036546f, 0.809000f,-0.587800f,0.000000f, 0.489685f,0.998195f},
{ 0.768531f,-0.586774f,-0.036546f, 0.809000f,-0.587800f,0.000000f, 0.509685f,0.998195f},
{ 0.768531f,-0.586774f,-0.036546f, -0.553100f,-0.833100f,0.000000f, 0.509685f,0.998195f},
{ 0.797726f,-0.546588f,-0.036546f, 0.621500f,0.783400f,-0.000000f, 0.489685f,0.998195f},
{ -0.000000f,-0.186007f,0.291800f, 0.000000f,-0.064700f,0.997900f, 0.578797f,0.430000f},
{ 0.100871f,-0.456960f,0.255800f, 0.055400f,-0.170600f,0.983800f, 0.692151f,0.472203f},
{ 0.188343f,-0.428536f,0.255800f, 0.055400f,-0.170600f,0.983800f, 0.680260f,0.508801f},
{ 0.110700f,-0.150036f,-0.036547f, 0.587800f,-0.809000f,0.000200f, 0.185085f,0.998195f},
{ 0.110700f,-0.150035f,0.291800f, 0.587800f,-0.809000f,0.000000f, 0.185085f,0.867195f},
{ 0.188343f,-0.428536f,0.255800f, 0.963400f,0.267900f,-0.000600f, 0.295842f,0.875267f},
{ -0.000000f,-0.186007f,0.291800f, 0.000000f,-1.000000f,0.000000f, 0.814285f,0.867195f},
{ -0.000000f,-0.186008f,-0.036547f, -0.000000f,-1.000000f,0.000200f, 0.814285f,0.998195f},
{ 0.100871f,-0.456960f,0.255800f, -0.936900f,-0.349500f,-0.000600f, 0.702373f,0.875267f},
{ 0.275510f,-0.926031f,0.104601f, -0.937200f,-0.348700f,0.001000f, 0.509685f,0.901977f},
{ 0.214359f,-0.760756f,0.176601f, -0.937300f,-0.348600f,0.002600f, 0.578849f,0.888510f},
{ 0.275081f,-0.741025f,0.176601f, 0.094900f,-0.292000f,0.951700f, 0.810991f,0.545091f},
{ 0.214359f,-0.760756f,0.176601f, 0.094900f,-0.292000f,0.951700f, 0.819246f,0.519685f},
{ 0.275081f,-0.741025f,0.176601f, 0.963200f,0.268800f,0.002600f, 0.420115f,0.888510f},
{ 0.322747f,-0.910681f,0.104601f, 0.117000f,-0.360000f,0.925600f, 0.881967f,0.565034f},
{ 0.275510f,-0.926031f,0.104601f, 0.117000f,-0.360000f,0.925600f, 0.888389f,0.545270f},
{ 0.322747f,-0.910681f,0.104601f, 0.963200f,0.268700f,0.001000f, 0.489685f,0.901977f},
{ 0.322747f,-0.910681f,0.104601f, 0.309000f,-0.951000f,0.000000f, 0.489685f,0.901977f},
{ 0.275510f,-0.926031f,0.104601f, 0.309000f,-0.951000f,0.000000f, 0.509685f,0.901977f},
{ 0.322747f,-0.910682f,-0.036545f, 0.309000f,-0.951000f,0.000000f, 0.489685f,0.998195f},
{ 0.275510f,-0.926031f,-0.036545f, 0.309000f,-0.951000f,0.000000f, 0.509685f,0.998195f},
{ 0.275510f,-0.926031f,-0.036545f, -0.937200f,-0.348900f,0.000000f, 0.509685f,0.998195f},
{ 0.322747f,-0.910682f,-0.036545f, 0.963300f,0.268500f,-0.000000f, 0.489685f,0.998195f},
{ -0.110700f,-0.150035f,0.291800f, -0.038000f,-0.052300f,0.997900f, 0.563748f,0.383684f},
{ -0.188343f,-0.428536f,0.255800f, -0.055400f,-0.170600f,0.983800f, 0.680260f,0.351199f},
{ -0.100870f,-0.456960f,0.255800f, -0.055400f,-0.170600f,0.983800f, 0.692151f,0.387797f},
{ -0.000000f,-0.186008f,-0.036547f, -0.000000f,-1.000000f,0.000200f, 0.185085f,0.998195f},
{ -0.000000f,-0.186007f,0.291800f, 0.000000f,-1.000000f,0.000000f, 0.185085f,0.867195f},
{ -0.100870f,-0.456960f,0.255800f, 0.936900f,-0.349500f,-0.000600f, 0.295842f,0.875267f},
{ -0.110700f,-0.150035f,0.291800f, -0.587800f,-0.809000f,0.000000f, 0.814285f,0.867195f},
{ -0.110700f,-0.150036f,-0.036547f, -0.587800f,-0.809000f,0.000200f, 0.814285f,0.998195f},
{ -0.188343f,-0.428536f,0.255800f, -0.963400f,0.267900f,-0.000600f, 0.702373f,0.875267f},
{ -0.322747f,-0.910681f,0.104601f, -0.963200f,0.268700f,0.001000f, 0.509685f,0.901977f},
{ -0.275081f,-0.741025f,0.176601f, -0.963200f,0.268800f,0.002600f, 0.578849f,0.888510f},
{ -0.214359f,-0.760756f,0.176601f, -0.094900f,-0.292000f,0.951700f, 0.819246f,0.340315f},
{ -0.275081f,-0.741025f,0.176601f, -0.094900f,-0.292000f,0.951700f, 0.810991f,0.314909f},
{ -0.214359f,-0.760756f,0.176601f, 0.937300f,-0.348600f,0.002600f, 0.420115f,0.888510f},
{ -0.275510f,-0.926031f,0.104601f, -0.117000f,-0.360000f,0.925600f, 0.888389f,0.314730f},
{ -0.322747f,-0.910681f,0.104601f, -0.117000f,-0.360000f,0.925600f, 0.881967f,0.294966f},
{ -0.275510f,-0.926031f,0.104601f, 0.937200f,-0.348700f,0.001000f, 0.489685f,0.901977f},
{ -0.275510f,-0.926031f,0.104601f, -0.309000f,-0.951000f,0.000000f, 0.489685f,0.901977f},
{ -0.322747f,-0.910681f,0.104601f, -0.309000f,-0.951000f,0.000000f, 0.509685f,0.901977f},
{ -0.275510f,-0.926031f,-0.036545f, -0.309000f,-0.951000f,0.000000f, 0.489685f,0.998195f},
{ -0.322747f,-0.910682f,-0.036545f, -0.309000f,-0.951000f,0.000000f, 0.509685f,0.998195f},
{ -0.322747f,-0.910682f,-0.036545f, -0.963300f,0.268500f,-0.000000f, 0.509685f,0.998195f},
{ -0.275510f,-0.926031f,-0.036545f, 0.937200f,-0.348900f,0.000000f, 0.489685f,0.998195f},
{ -0.179117f,-0.055860f,0.291800f, -0.061500f,-0.020000f,0.997900f, 0.524350f,0.355060f},
{ -0.405617f,-0.235531f,0.255800f, -0.145100f,-0.105400f,0.983800f, 0.599516f,0.260295f},
{ -0.351555f,-0.309946f,0.255800f, -0.145100f,-0.105400f,0.983800f, 0.630647f,0.282913f},
{ -0.110700f,-0.150036f,-0.036547f, -0.587800f,-0.809000f,0.000200f, 0.185085f,0.998195f},
{ -0.110700f,-0.150035f,0.291800f, -0.587800f,-0.809000f,0.000000f, 0.185085f,0.867195f},
{ -0.351555f,-0.309946f,0.255800f, 0.552600f,-0.833500f,-0.000600f, 0.295842f,0.875267f},
{ -0.179117f,-0.055860f,0.291800f, -0.951100f,-0.309000f,0.000000f, 0.814285f,0.867195f},
{ -0.179117f,-0.055861f,-0.036547f, -0.951100f,-0.309000f,0.000200f, 0.814285f,0.998195f},
{ -0.405617f,-0.235531f,0.255800f, -0.622000f,0.783000f,-0.000600f, 0.702373f,0.875267f},
{ -0.797725f,-0.546587f,0.104601f, -0.621300f,0.783500f,0.001000f, 0.509685f,0.901977f},
{ -0.659450f,-0.437352f,0.176600f, -0.621200f,0.783600f,0.002600f, 0.578849f,0.888510f},
{ -0.621922f,-0.489010f,0.176600f, -0.248400f,-0.180500f,0.951700f, 0.705559f,0.169795f},
{ -0.659450f,-0.437352f,0.176600f, -0.248400f,-0.180500f,0.951700f, 0.683948f,0.154094f},
{ -0.621922f,-0.489010f,0.176600f, 0.553400f,-0.832900f,0.002600f, 0.420115f,0.888510f},
{ -0.768531f,-0.586773f,0.104601f, -0.306200f,-0.222500f,0.925600f, 0.746459f,0.108456f},
{ -0.797725f,-0.546587f,0.104601f, -0.306200f,-0.222500f,0.925600f, 0.729647f,0.096241f},
{ -0.768531f,-0.586773f,0.104601f, 0.553300f,-0.833000f,0.001000f, 0.489685f,0.901977f},
{ -0.768531f,-0.586773f,0.104601f, -0.809000f,-0.587700f,0.000000f, 0.489685f,0.901977f},
{ -0.797725f,-0.546587f,0.104601f, -0.809000f,-0.587700f,0.000000f, 0.509685f,0.901977f},
{ -0.768531f,-0.586774f,-0.036546f, -0.809000f,-0.587800f,0.000000f, 0.489685f,0.998195f},
{ -0.797725f,-0.546588f,-0.036546f, -0.809000f,-0.587800f,0.000000f, 0.509685f,0.998195f},
{ -0.797725f,-0.546588f,-0.036546f, -0.621500f,0.783400f,-0.000000f, 0.509685f,0.998195f},
{ -0.768531f,-0.586774f,-0.036546f, 0.553100f,-0.833100f,0.000000f, 0.489685f,0.998195f},
{ -0.179117f,0.060547f,0.291799f, -0.061500f,0.020000f,0.997900f, 0.475650f,0.355060f},
{ -0.467958f,0.048334f,0.255799f, -0.179400f,0.000000f,0.983800f, 0.480760f,0.234212f},
{ -0.467958f,-0.043648f,0.255800f, -0.179400f,0.000000f,0.983800f, 0.519241f,0.234212f},
{ -0.179117f,-0.055861f,-0.036547f, -0.951100f,-0.309000f,0.000200f, 0.185085f,0.998195f},
{ -0.179117f,-0.055860f,0.291800f, -0.951100f,-0.309000f,0.000000f, 0.185085f,0.867195f},
{ -0.467958f,-0.043648f,0.255800f, -0.042900f,-0.999100f,-0.000600f, 0.295842f,0.875267f},
{ -0.179117f,0.060547f,0.291799f, -0.951100f,0.309000f,-0.000000f, 0.814285f,0.867195f},
{ -0.179117f,0.060546f,-0.036547f, -0.951100f,0.309000f,0.000200f, 0.814285f,0.998195f},
{ -0.467958f,0.048334f,0.255799f, -0.042900f,0.999100f,-0.000600f, 0.702373f,0.875267f},
{ -0.968000f,0.027179f,0.104599f, -0.042100f,0.999100f,0.001000f, 0.509685f,0.901977f},
{ -0.791931f,0.034269f,0.176599f, -0.041900f,0.999100f,0.002600f, 0.578849f,0.888510f},
{ -0.791931f,-0.029583f,0.176600f, -0.307100f,0.000000f,0.951700f, 0.513357f,0.098665f},
{ -0.791931f,0.034269f,0.176599f, -0.307100f,0.000000f,0.951700f, 0.486643f,0.098665f},
{ -0.791931f,-0.029583f,0.176600f, -0.041900f,-0.999100f,0.002600f, 0.420115f,0.888510f},
{ -0.968000f,-0.022493f,0.104600f, -0.378500f,0.000000f,0.925600f, 0.510390f,0.025000f},
{ -0.968000f,0.027179f,0.104599f, -0.378500f,0.000000f,0.925600f, 0.489610f,0.025000f},
{ -0.968000f,-0.022493f,0.104600f, -0.042100f,-0.999100f,0.001000f, 0.489685f,0.901977f},
{ -0.968000f,-0.022493f,0.104600f, -1.000000f,-0.000000f,-0.000000f, 0.489685f,0.901977f},
{ -0.968000f,0.027179f,0.104599f, -1.000000f,-0.000000f,-0.000000f, 0.509685f,0.901977f},
{ -0.968000f,-0.022494f,-0.036547f, -1.000000f,-0.000000f,-0.000000f, 0.489685f,0.998195f},
{ -0.968000f,0.027178f,-0.036547f, -1.000000f,-0.000000f,-0.000000f, 0.509685f,0.998195f},
{ -0.968000f,0.027178f,-0.036547f, -0.042300f,0.999100f,-0.000000f, 0.509685f,0.998195f},
{ -0.968000f,-0.022494f,-0.036547f, -0.042300f,-0.999100f,0.000000f, 0.489685f,0.998195f},
{ -0.110700f,0.154722f,0.291799f, -0.038000f,0.052300f,0.997900f, 0.436252f,0.383684f},
{ -0.351555f,0.314633f,0.255799f, -0.145100f,0.105400f,0.983800f, 0.369353f,0.282913f},
{ -0.405617f,0.240217f,0.255799f, -0.145100f,0.105400f,0.983800f, 0.400485f,0.260295f},
{ -0.179117f,0.060546f,-0.036547f, -0.951100f,0.309000f,0.000200f, 0.185085f,0.998195f},
{ -0.179117f,0.060547f,0.291799f, -0.951100f,0.309000f,-0.000000f, 0.185085f,0.867195f},
{ -0.405617f,0.240217f,0.255799f, -0.622000f,-0.783000f,-0.000600f, 0.295842f,0.875267f},
{ -0.110700f,0.154722f,0.291799f, -0.587800f,0.809000f,-0.000000f, 0.814285f,0.867195f},
{ -0.110700f,0.154721f,-0.036547f, -0.587800f,0.809000f,0.000200f, 0.814285f,0.998195f},
{ -0.351555f,0.314633f,0.255799f, 0.552600f,0.833500f,-0.000600f, 0.702373f,0.875267f},
{ -0.768531f,0.591459f,0.104598f, 0.553300f,0.833000f,0.001000f, 0.509685f,0.901977f},
{ -0.621922f,0.493696f,0.176599f, 0.553400f,0.832900f,0.002600f, 0.578849f,0.888510f},
{ -0.659450f,0.442038f,0.176599f, -0.248400f,0.180500f,0.951700f, 0.316052f,0.154094f},
{ -0.621922f,0.493696f,0.176599f, -0.248400f,0.180500f,0.951700f, 0.294441f,0.169795f},
{ -0.659450f,0.442038f,0.176599f, -0.621200f,-0.783600f,0.002600f, 0.420115f,0.888510f},
{ -0.797726f,0.551273f,0.104598f, -0.306200f,0.222500f,0.925600f, 0.270353f,0.096241f},
{ -0.768531f,0.591459f,0.104598f, -0.306200f,0.222500f,0.925600f, 0.253541f,0.108456f},
{ -0.797726f,0.551273f,0.104598f, -0.621300f,-0.783500f,0.001000f, 0.489685f,0.901977f},
{ -0.797726f,0.551273f,0.104598f, -0.809000f,0.587800f,-0.000000f, 0.489685f,0.901977f},
{ -0.768531f,0.591459f,0.104598f, -0.809000f,0.587800f,-0.000000f, 0.509685f,0.901977f},
{ -0.797726f,0.551273f,-0.036548f, -0.809000f,0.587800f,-0.000000f, 0.489685f,0.998195f},
{ -0.768531f,0.591458f,-0.036548f, -0.809000f,0.587800f,-0.000000f, 0.509685f,0.998195f},
{ -0.768531f,0.591458f,-0.036548f, 0.553100f,0.833100f,-0.000000f, 0.509685f,0.998195f},
{ -0.797726f,0.551273f,-0.036548f, -0.621500f,-0.783400f,0.000000f, 0.489685f,0.998195f},
{ 0.000000f,0.190694f,0.291799f, -0.000000f,0.064700f,0.997900f, 0.421203f,0.430000f},
{ -0.100871f,0.461647f,0.255799f, -0.055400f,0.170600f,0.983800f, 0.307849f,0.387797f},
{ -0.188344f,0.433223f,0.255799f, -0.055400f,0.170600f,0.983800f, 0.319740f,0.351199f},
{ -0.110700f,0.154721f,-0.036547f, -0.587800f,0.809000f,0.000200f, 0.185085f,0.998195f},
{ -0.110700f,0.154722f,0.291799f, -0.587800f,0.809000f,-0.000000f, 0.185085f,0.867195f},
{ -0.188344f,0.433223f,0.255799f, -0.963400f,-0.267900f,-0.000600f, 0.295842f,0.875267f},
{ 0.000000f,0.190694f,0.291799f, -0.000000f,1.000000f,-0.000000f, 0.814285f,0.867195f},
{ 0.000000f,0.190693f,-0.036547f, -0.000000f,1.000000f,0.000200f, 0.814285f,0.998195f},
{ -0.100871f,0.461647f,0.255799f, 0.936900f,0.349500f,-0.000600f, 0.702373f,0.875267f},
{ -0.275510f,0.930717f,0.104598f, 0.937200f,0.348700f,0.001000f, 0.509685f,0.901977f},
{ -0.214359f,0.765443f,0.176598f, 0.937300f,0.348600f,0.002600f, 0.578849f,0.888510f},
{ -0.275081f,0.745711f,0.176598f, -0.094900f,0.292000f,0.951700f, 0.189009f,0.314909f},
{ -0.214359f,0.765443f,0.176598f, -0.094900f,0.292000f,0.951700f, 0.180755f,0.340315f},
{ -0.275081f,0.745711f,0.176598f, -0.963200f,-0.268800f,0.002600f, 0.420115f,0.888510f},
{ -0.322747f,0.915367f,0.104598f, -0.117000f,0.360000f,0.925600f, 0.118033f,0.294966f},
{ -0.275510f,0.930717f,0.104598f, -0.117000f,0.360000f,0.925600f, 0.111611f,0.314730f},
{ -0.322747f,0.915367f,0.104598f, -0.963200f,-0.268700f,0.001000f, 0.489685f,0.901977f},
{ -0.322747f,0.915367f,0.104598f, -0.309000f,0.951000f,-0.000000f, 0.489685f,0.901977f},
{ -0.275510f,0.930717f,0.104598f, -0.309000f,0.951000f,-0.000000f, 0.509685f,0.901977f},
{ -0.322747f,0.915366f,-0.036549f, -0.309000f,0.951000f,-0.000000f, 0.489685f,0.998195f},
{ -0.275510f,0.930716f,-0.036549f, -0.309000f,0.951100f,-0.000000f, 0.509685f,0.998195f},
{ -0.275510f,0.930716f,-0.036549f, 0.937200f,0.348900f,-0.000000f, 0.509685f,0.998195f},
{ -0.322747f,0.915366f,-0.036549f, -0.963300f,-0.268500f,0.000000f, 0.489685f,0.998195f},
{ 0.110700f,0.154722f,0.291799f, 0.038000f,0.052300f,0.997900f, 0.436252f,0.476316f},
{ 0.188343f,0.433223f,0.255799f, 0.055400f,0.170600f,0.983800f, 0.319740f,0.508801f},
{ 0.100870f,0.461647f,0.255799f, 0.055400f,0.170600f,0.983800f, 0.307849f,0.472203f},
{ 0.000000f,0.190693f,-0.036547f, -0.000000f,1.000000f,0.000200f, 0.185085f,0.998195f},
{ 0.000000f,0.190694f,0.291799f, -0.000000f,1.000000f,-0.000000f, 0.185085f,0.867195f},
{ 0.100870f,0.461647f,0.255799f, -0.936900f,0.349500f,-0.000600f, 0.295842f,0.875267f},
{ 0.110700f,0.154722f,0.291799f, 0.587800f,0.809000f,-0.000000f, 0.814285f,0.867195f},
{ 0.110700f,0.154721f,-0.036547f, 0.587800f,0.809000f,0.000200f, 0.814285f,0.998195f},
{ 0.188343f,0.433223f,0.255799f, 0.963400f,-0.267900f,-0.000600f, 0.702373f,0.875267f},
{ 0.322747f,0.915367f,0.104598f, 0.963200f,-0.268700f,0.001000f, 0.509685f,0.901977f},
{ 0.275081f,0.745711f,0.176598f, 0.963200f,-0.268800f,0.002600f, 0.578849f,0.888510f},
{ 0.214359f,0.765443f,0.176598f, 0.094900f,0.292000f,0.951700f, 0.180755f,0.519685f},
{ 0.275081f,0.745711f,0.176598f, 0.094900f,0.292000f,0.951700f, 0.189009f,0.545091f},
{ 0.214359f,0.765443f,0.176598f, -0.937300f,0.348600f,0.002600f, 0.420115f,0.888510f},
{ 0.275510f,0.930717f,0.104598f, 0.117000f,0.360000f,0.925600f, 0.111611f,0.545270f},
{ 0.322747f,0.915367f,0.104598f, 0.117000f,0.360000f,0.925600f, 0.118033f,0.565034f},
{ 0.275510f,0.930717f,0.104598f, -0.937200f,0.348700f,0.001000f, 0.489685f,0.901977f},
{ 0.275510f,0.930717f,0.104598f, 0.309000f,0.951000f,-0.000000f, 0.489685f,0.901977f},
{ 0.322747f,0.915367f,0.104598f, 0.309000f,0.951000f,-0.000000f, 0.509685f,0.901977f},
{ 0.275510f,0.930716f,-0.036549f, 0.309000f,0.951000f,-0.000000f, 0.489685f,0.998195f},
{ 0.322747f,0.915366f,-0.036549f, 0.309000f,0.951000f,-0.000000f, 0.509685f,0.998195f},
{ 0.322747f,0.915366f,-0.036549f, 0.963300f,-0.268500f,0.000000f, 0.509685f,0.998195f},
{ 0.275510f,0.930716f,-0.036549f, -0.937200f,0.348900f,-0.000000f, 0.489685f,0.998195f},
{ 0.405617f,0.240217f,0.255799f, 0.145100f,0.105400f,0.983800f, 0.400485f,0.599705f},
{ 0.351555f,0.314633f,0.255799f, 0.145100f,0.105400f,0.983800f, 0.369353f,0.577087f},
{ 0.110700f,0.154721f,-0.036547f, 0.587800f,0.809000f,0.000200f, 0.185085f,0.998195f},
{ 0.110700f,0.154722f,0.291799f, 0.587800f,0.809000f,-0.000000f, 0.185085f,0.867195f},
{ 0.351555f,0.314633f,0.255799f, -0.552600f,0.833500f,-0.000600f, 0.295842f,0.875267f},
{ 0.179117f,0.060547f,0.291799f, 0.951100f,0.309000f,-0.000000f, 0.814285f,0.867195f},
{ 0.179117f,0.060546f,-0.036547f, 0.951100f,0.309000f,0.000200f, 0.814285f,0.998195f},
{ 0.405617f,0.240217f,0.255799f, 0.622000f,-0.783000f,-0.000600f, 0.702373f,0.875267f},
{ 0.797726f,0.551273f,0.104598f, 0.621300f,-0.783500f,0.001000f, 0.509685f,0.901977f},
{ 0.659450f,0.442038f,0.176599f, 0.621200f,-0.783600f,0.002600f, 0.578849f,0.888510f},
{ 0.621922f,0.493696f,0.176599f, 0.248400f,0.180500f,0.951700f, 0.294441f,0.690205f},
{ 0.659450f,0.442038f,0.176599f, 0.248400f,0.180500f,0.951700f, 0.316052f,0.705906f},
{ 0.621922f,0.493696f,0.176599f, -0.553400f,0.832900f,0.002600f, 0.420115f,0.888510f},
{ 0.768531f,0.591459f,0.104598f, 0.306200f,0.222500f,0.925600f, 0.253541f,0.751545f},
{ 0.797726f,0.551273f,0.104598f, 0.306200f,0.222500f,0.925600f, 0.270353f,0.763759f},
{ 0.768531f,0.591459f,0.104598f, -0.553300f,0.833000f,0.001000f, 0.489685f,0.901977f},
{ 0.768531f,0.591459f,0.104598f, 0.809000f,0.587800f,-0.000000f, 0.489685f,0.901977f},
{ 0.797726f,0.551273f,0.104598f, 0.809000f,0.587800f,-0.000000f, 0.509685f,0.901977f},
{ 0.768531f,0.591459f,-0.036548f, 0.809000f,0.587800f,-0.000000f, 0.489685f,0.998195f},
{ 0.797726f,0.551273f,-0.036548f, 0.809000f,0.587800f,-0.000000f, 0.509685f,0.998195f},
{ 0.797726f,0.551273f,-0.036548f, 0.621500f,-0.783400f,0.000000f, 0.509685f,0.998195f},
{ 0.768531f,0.591459f,-0.036548f, -0.553100f,0.833100f,-0.000000f, 0.489685f,0.998195f}
};

WORD triggerStarIndices[510]=
{
   0,1,2,2,1,3,4,2,3,5,6,7,8,9,10,
   9,11,12,13,4,14,3,14,4,7,15,5,12,10,9,
   16,13,17,14,17,13,15,18,5,19,20,21,22,21,20,
   23,11,9,5,18,24,0,25,1,1,25,26,27,1,26,
   28,29,30,31,32,33,32,34,35,36,27,37,26,37,27,
   30,38,28,35,33,32,39,36,40,37,40,36,38,41,28,
   42,43,44,45,44,43,46,34,32,28,41,47,0,48,25,
   25,48,49,50,25,49,51,52,53,54,55,56,55,57,58,
   59,50,60,49,60,50,53,61,51,58,56,55,62,59,63,
   60,63,59,61,64,51,65,66,67,68,67,66,69,57,55,
   51,64,70,0,71,48,48,71,72,73,48,72,74,75,76,
   77,78,79,78,80,81,82,73,83,72,83,73,76,84,74,
   81,79,78,85,82,86,83,86,82,84,87,74,88,89,90,
   91,90,89,92,80,78,74,87,93,0,94,71,71,94,95,
   96,71,95,97,98,99,100,101,102,101,103,104,105,96,106,
   95,106,96,99,107,97,104,102,101,108,105,109,106,109,105,
   107,110,97,111,112,113,114,113,112,115,103,101,97,110,116,
   0,117,94,94,117,118,119,94,118,120,121,122,123,124,125,
   124,126,127,128,119,129,118,129,119,122,130,120,127,125,124,
   131,128,132,129,132,128,130,133,120,134,135,136,137,136,135,
   138,126,124,120,133,139,0,140,117,117,140,141,142,117,141,
   143,144,145,146,147,148,147,149,150,151,142,152,141,152,142,
   145,153,143,150,148,147,154,151,155,152,155,151,153,156,143,
   157,158,159,160,159,158,161,149,147,143,156,162,0,163,140,
   140,163,164,165,140,164,166,167,168,169,170,171,170,172,173,
   174,165,175,164,175,165,168,176,166,173,171,170,177,174,178,
   175,178,174,176,179,166,180,181,182,183,182,181,184,172,170,
   166,179,185,0,186,163,163,186,187,188,163,187,189,190,191,
   192,193,194,193,195,196,197,188,198,187,198,188,191,199,189,
   196,194,193,200,197,201,198,201,197,199,202,189,203,204,205,
   206,205,204,207,195,193,189,202,208,0,2,186,186,2,209,
   210,186,209,211,212,213,214,215,216,215,217,218,219,210,220,
   209,220,210,213,221,211,218,216,215,222,219,223,220,223,219,
   221,224,211,225,226,227,228,227,226,229,217,215,211,224,230 
};