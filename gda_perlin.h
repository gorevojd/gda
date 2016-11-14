#ifdef __cplusplus
extern "C" float gda_perlin_noise3(float x, float y, float z, int wraw_x=0, int wraw_y=0, int wrap_z=0);
#else
extern float gda_perlin_noise3(float x, float y, float z, int wraw_x, int wraw_y, int wrap_z);
#endif

#define GDA_PERLIN_IMPLEMENTATION
#ifdef GDA_PERLIN_IMPLEMENTATION

#include <math.h>

static int gda__perlin_random_table[512] = {
	48, 110, 70, 38, 71, 19, 157, 55,
	18, 248, 14, 183, 222, 0, 255, 143,
	84, 99, 51, 150, 162, 231, 134, 91,
	251, 56, 60, 32, 39, 109, 247, 203,
	152, 172, 80, 94, 141, 214, 206, 219,
	78, 161, 191, 123, 249, 100, 35, 42,
	177, 45, 15, 140, 199, 146, 198, 241,
	75, 2, 163, 245, 124, 165, 43, 64,
	86, 127, 115, 139, 132, 83, 180, 37,
	190, 20, 30, 129, 108, 225, 49, 131,
	6, 156, 244, 21, 215, 130, 212, 81,
	7, 62, 102, 226, 137, 174, 254, 33,
	145, 181, 166, 121, 63, 85, 90, 171,
	23, 88, 9, 106, 72, 192, 218, 153,
	202, 59, 234, 242, 237, 25, 77, 119,
	133, 26, 29, 50, 73, 34, 118, 200,
	253, 92, 204, 97, 167, 24, 28, 41,
	65, 93, 120, 74, 173, 95, 201, 113,
	223, 168, 122, 47, 176, 213, 169, 216,
	195, 67, 126, 188, 1, 69, 136, 221,
	5, 128, 101, 236, 135, 54, 232, 96,
	144, 53, 4, 230, 10, 36, 227, 125,
	233, 229, 22, 46, 61, 159, 186, 111,
	142, 207, 185, 104, 211, 228, 79, 117,
	44, 179, 155, 184, 224, 187, 89, 158,
	148, 154, 217, 238, 40, 193, 13, 151,
	11, 82, 12, 194, 3, 205, 182, 112,
	27, 209, 208, 16, 76, 178, 189, 116,
	220, 105, 107, 114, 87, 52, 197, 57,
	31, 175, 240, 239, 252, 196, 250, 210,
	149, 246, 243, 8, 68, 170, 17, 138,
	98, 66, 160, 103, 235, 164, 147, 58,

	48, 110, 70, 38, 71, 19, 157, 55,
	18, 248, 14, 183, 222, 0, 255, 143,
	84, 99, 51, 150, 162, 231, 134, 91,
	251, 56, 60, 32, 39, 109, 247, 203,
	152, 172, 80, 94, 141, 214, 206, 219,
	78, 161, 191, 123, 249, 100, 35, 42,
	177, 45, 15, 140, 199, 146, 198, 241,
	75, 2, 163, 245, 124, 165, 43, 64,
	86, 127, 115, 139, 132, 83, 180, 37,
	190, 20, 30, 129, 108, 225, 49, 131,
	6, 156, 244, 21, 215, 130, 212, 81,
	7, 62, 102, 226, 137, 174, 254, 33,
	145, 181, 166, 121, 63, 85, 90, 171,
	23, 88, 9, 106, 72, 192, 218, 153,
	202, 59, 234, 242, 237, 25, 77, 119,
	133, 26, 29, 50, 73, 34, 118, 200,
	253, 92, 204, 97, 167, 24, 28, 41,
	65, 93, 120, 74, 173, 95, 201, 113,
	223, 168, 122, 47, 176, 213, 169, 216,
	195, 67, 126, 188, 1, 69, 136, 221,
	5, 128, 101, 236, 135, 54, 232, 96,
	144, 53, 4, 230, 10, 36, 227, 125,
	233, 229, 22, 46, 61, 159, 186, 111,
	142, 207, 185, 104, 211, 228, 79, 117,
	44, 179, 155, 184, 224, 187, 89, 158,
	148, 154, 217, 238, 40, 193, 13, 151,
	11, 82, 12, 194, 3, 205, 182, 112,
	27, 209, 208, 16, 76, 178, 189, 116,
	220, 105, 107, 114, 87, 52, 197, 57,
	31, 175, 240, 239, 252, 196, 250, 210,
	149, 246, 243, 8, 68, 170, 17, 138,
	98, 66, 160, 103, 235, 164, 147, 58
};

static float gda__perlin_lerp(float a, float b, float t){
	return(a + (b - a) * t);
}

static float gda__perlin_grad(int hash, float x, float y, float z){
	
	static float basis[12][4] = {
		{ 1, 1, 0 },
		{ -1, 1, 0 },
		{ 1, -1, 0 },
		{ -1, -1, 0 },
		{ 1, 0, 1 },
		{ -1, 0, 1 },
		{ 1, 0, -1 },
		{ -1, 0, -1 },
		{ 0, 1, 1 },
		{ 0, -1, 1 },
		{ 0, 1, -1 },
		{ 0, -1, -1 },
	};

	static unsigned char indices[64] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
		0, 9, 1, 11,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
	};

	float* grad = basis[indices[hash & 63]];
	return(grad[0] * x + grad[1] * y + grad[2] * z);
}

float gda_perlin_noise3(float x, float y, float z, int wrap_x, int wrap_y, int wrap_z){
	float u, v, w;
	float n000, n001, n010, n011, n100, n101, n110, n111;
	float n00, n01, n10, n11;
	float n0, n1;

	unsigned int x_mask = (wrap_x - 1) & 255;
	unsigned int y_mask = (wrap_y - 1) & 255;
	unsigned int z_mask = (wrap_z - 1) & 255;

	int px = (int)floor(x);
	int py = (int)floor(y);
	int pz = (int)floor(z);

	int x0 = px & x_mask, x1 = (px + 1) & x_mask;
	int y0 = py & y_mask, y1 = (py + 1) & y_mask;
	int z0 = pz & z_mask, z1 = (pz + 1) & z_mask;
	int r0, r1, r00, r01, r10, r11;

#define gda__perlin_ease(a) (((a * 6 - 15) * a + 10) * a * a * a)

	x -= px; u = gda__perlin_ease(x);
	y -= py; v = gda__perlin_ease(y);
	z -= pz; w = gda__perlin_ease(z);

	r0 = gda__perlin_random_table[x0];
	r1 = gda__perlin_random_table[x1];

	r00 = gda__perlin_random_table[r0 + y0];
	r01 = gda__perlin_random_table[r0 + y1];
	r10 = gda__perlin_random_table[r1 + y0];
	r11 = gda__perlin_random_table[r1 + y1];

	n000 = gda__perlin_grad(gda__perlin_random_table[r00 + z0], x, y, z);
	n001 = gda__perlin_grad(gda__perlin_random_table[r00 + z1], x, y, z - 1);
	n010 = gda__perlin_grad(gda__perlin_random_table[r01 + z0], x, y - 1, z);
	n011 = gda__perlin_grad(gda__perlin_random_table[r01 + z1], x, y - 1, z - 1);
	n100 = gda__perlin_grad(gda__perlin_random_table[r10 + z0], x - 1, y, z);
	n101 = gda__perlin_grad(gda__perlin_random_table[r10 + z1], x - 1, y, z - 1);
	n110 = gda__perlin_grad(gda__perlin_random_table[r11 + z0], x - 1, y - 1, z);
	n111 = gda__perlin_grad(gda__perlin_random_table[r11 + z1], x - 1, y - 1, z - 1);

	n00 = gda__perlin_lerp(n000, n001, w);
	n01 = gda__perlin_lerp(n010, n011, w);
	n10 = gda__perlin_lerp(n100, n101, w);
	n11 = gda__perlin_lerp(n110, n111, w);

	n0 = gda__perlin_lerp(n00, n01, v);
	n1 = gda__perlin_lerp(n10, n11, v);

	return(gda__perlin_lerp(n0, n1, u));
}

#endif