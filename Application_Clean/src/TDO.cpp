#include "TDO.h"

#pragma region TDO Definitions



void TDO::updateMatrix() {
	_matrixS = glm::scale(_matrix, _scale);
}

void TDO::move(const glm::vec3& in) {
	_matrix = glm::translate(_matrix, in);
	updateMatrix();
}
void TDO::rotate(float speed, const glm::vec3& axis) {
	_matrix = glm::rotate(_matrix, speed, axis);
	updateMatrix();
}
void TDO::setPos(const glm::vec3& in) {
	_matrix[3] = glm::vec4(in,_matrix[3][3]);
	updateMatrix();
}
void TDO::setScale(const glm::vec3& in) {
	_scale = in;
	updateMatrix();
}
void TDO::setScale(float in) {
	setScale(glm::vec3{ in });
}

void TDO::setColor(glm::vec3 in) {
	_color = in;
}

#pragma endregion

#pragma region Cube Definitions


void cube::addTan(std::vector<float>& outvec,float* fin, unsigned flen, unsigned* iin, unsigned ilen) {

	outvec.shrink_to_fit();
	// 192 in div by 8 to remove stride for vertexes then times by 11 for new stride = 264
	// assign is used instead of reserve as we need the vector to be pre populated to memmove to it
	outvec.assign(264, 0.f);
	std::vector<vertex> temp;
	temp.reserve(flen / 8);
	vertex V;
	V.tan  = glm::vec3{0.f};
	for (unsigned i = 0; i < flen; i += 8) {
		V.pos  = glm::vec3{fin[i+0], fin[i+1], fin[i+2]};
		V.norm = glm::vec3{fin[i+3], fin[i+4], fin[i+5]};
		V.uv   = glm::vec2{fin[i+6], fin[i+7]};
		temp.push_back(V);
	}
	//loop the vertexes, add tangents accordingly
	for (unsigned i = 0; i < ilen; i += 3) {
		vertex& v1 = temp.at(iin[i+0]);
		vertex& v2 = temp.at(iin[i+1]);
		vertex& v3 = temp.at(iin[i+2]);

		glm::vec3 dp1 = v2.pos - v1.pos;
		glm::vec3 dp2 = v3.pos - v1.pos;

		glm::vec2 uv1 = v2.uv - v1.uv;
		glm::vec2 uv2 = v3.uv - v1.uv;

		float r = 1.f / ((uv1.x * uv2.y) - (uv1.y * uv2.x));
		//set tangent
		glm::vec3 tan = r * ((dp1 * uv2.y) - (dp2 * uv1.y));
		v1.tan += tan;
		v2.tan += tan;
		v3.tan += tan;
	}
	//normalize any vertexes that had multiple tangents added, then push all the data back to the outvec
	for (vertex& v : temp) {
		v.tan = glm::normalize(v.tan);
		/*
		outvec.push_back(v.pos.x);
		outvec.push_back(v.pos.y);
		outvec.push_back(v.pos.z);
		outvec.push_back(v.norm.x);
		outvec.push_back(v.norm.y);
		outvec.push_back(v.norm.z);
		outvec.push_back(v.uv.x);
		outvec.push_back(v.uv.y);
		outvec.push_back(v.tan.x);
		outvec.push_back(v.tan.y);
		outvec.push_back(v.tan.z);
		*/
	}

	//works only bc both vectors, and all of the contents of vertex are all just contiguous floats underneath
	//don't use memcpy, crashes sometimes. memmove_s has never crashed and is still an order of magnitude faster
	memmove_s(outvec.data(), outvec.size() * sizeof(float), temp.data(), temp.size() * sizeof(vertex));
}



cube::cube(const glm::vec3& size, const glm::vec3& pos, float t) {
	_uvScale = t;
	_aabb = { size, -size };
	constexpr unsigned fsize = 192;
	constexpr unsigned isize = 36;
	float temp[fsize] = {
		//           XYZ sizes               normals                UV coords
		//   <---------------------->   <---------------->  <--------------------->
		//back
			 size.x, -size.y, -size.z,  0.0f,  0.0f, -1.0f, 0.0f      , 0.0f      ,
			-size.x, -size.y, -size.z,  0.0f,  0.0f, -1.0f, size.x * t, 0.0f      ,
			-size.x,  size.y, -size.z,  0.0f,  0.0f, -1.0f, size.x * t, size.y * t,
			 size.x,  size.y, -size.z,  0.0f,  0.0f, -1.0f, 0.0f      , size.y * t,
			 
	    //front
			 -size.x, -size.y,  size.z,  0.0f,  0.0f,  1.0f, 0.0f      , 0.0f     ,
			  size.x, -size.y,  size.z,  0.0f,  0.0f,  1.0f, size.x * t, 0.0f     ,
			  size.x,  size.y,  size.z,  0.0f,  0.0f,  1.0f, size.x * t, size.y * t,
			 -size.x,  size.y,  size.z,  0.0f,  0.0f,  1.0f, 0.0f      , size.y * t,
		//left
			 -size.x,  size.y,  size.z, -1.0f,  0.0f,  0.0f, 0.0f      , 0.0f      ,
			 -size.x,  size.y, -size.z, -1.0f,  0.0f,  0.0f, size.z * t, 0.0f      ,
			 -size.x, -size.y, -size.z, -1.0f,  0.0f,  0.0f, size.z * t, size.y * t,
			 -size.x, -size.y,  size.z, -1.0f,  0.0f,  0.0f, 0.0f      , size.y * t,
		//right
			 size.x,  size.y,  -size.z,  1.0f,  0.0f,  0.0f, 0.0f      , 0.0f      ,
			 size.x,  size.y,   size.z,  1.0f,  0.0f,  0.0f, size.z * t, 0.0f      ,
			 size.x, -size.y,   size.z,  1.0f,  0.0f,  0.0f, size.z * t, size.y * t,
			 size.x, -size.y,  -size.z,  1.0f,  0.0f,  0.0f, 0.0f      , size.y * t,
		//bottom
			 -size.x, -size.y, -size.z,  0.0f, -1.0f,  0.0f, 0.0f      , 0.0f      ,
			  size.x, -size.y, -size.z,  0.0f, -1.0f,  0.0f, size.x * t, 0.0f      ,
			  size.x, -size.y,  size.z,  0.0f, -1.0f,  0.0f, size.x * t, size.z * t,
			 -size.x, -size.y,  size.z,  0.0f, -1.0f,  0.0f, 0.0f      , size.z * t,
		//top	
			 -size.x,  size.y, -size.z,  0.0f,  1.0f,  0.0f, 0.0f      , 0.0f      ,
			  size.x,  size.y, -size.z,  0.0f,  1.0f,  0.0f, size.x * t, 0.0f      ,
			  size.x,  size.y,  size.z,  0.0f,  1.0f,  0.0f, size.x * t, size.z * t,
			 -size.x,  size.y,  size.z,  0.0f,  1.0f,  0.0f, 0.0f      , size.z * t
	};
	unsigned indices[36] = {
		1,2,3,
		1,3,0,

		5,6,7,
		5,7,4,

		11,8,9,
		11,9,10,

		15,12,13,
		15,13,14,

		23,22,21,
		23,21,20,

		16,17,18,
		16,18,19

	};

	//add tangent to cube
	std::vector<float> myvertexinfo{};

	addTan(myvertexinfo, temp, 192, indices, 36);




	// Create VAO
	// Cube
	glGenVertexArrays(1, &_VAO);  // generate 1 VAO buffer called VAO
	glGenBuffers(1, &_VBO);       // generate buffer called VBO  - this is for vertex data
	glGenBuffers(1, &_EBO);       // another called EBO for index info

	glBindVertexArray(_VAO);  // bind VAO
	// fill VBO with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);  // bind VBO
	glBufferData(GL_ARRAY_BUFFER, myvertexinfo.size() * sizeof(float), myvertexinfo.data(), GL_STATIC_DRAW);   // 'send' or copy the data in vertices from CPU to GPU, 'STATIC_DRAW' is a hint to OpenGL that we will not be updating the data in this buffer
	// fill EBO with index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);  // bind EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // 'send' or copy the data in indices from CPU to GPU


	// tell OpenGl how to interpret the data in VBO
	// This is 'stored' in VAO along with VBO so we only need to do this once and whenever we bind m_cubeVAO we bind the data and instructions on how to interpret
	// Whenever we bind VAO we are now dealing with VBO, EBO, and the following instructions. All with one command: glBindVertexArray(VAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); //params: at position 0, length , type (so here, 3 floats), not normalised, stride (6 floats in total per vertex), position to start - start at 0 here)
	glEnableVertexAttribArray(0);  // first attribute
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));  //params: at position 1, length , type (so here, 3 floats), not normalised, stride (6 floats in total per vertex), position to start - start at 3 here)
	glEnableVertexAttribArray(1); // 2nd attribute
	// uv attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));  //params: at position 1, length , type (so here, 3 floats), not normalised, stride (6 floats in total per vertex), position to start - start at 3 here)
	glEnableVertexAttribArray(2); // 3rd attribute

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));  //params: at position 1, length , type (so here, 3 floats), not normalised, stride (6 floats in total per vertex), position to start - start at 3 here)
	glEnableVertexAttribArray(3); // 4th attribute


	move(pos);
};

void cube::_simpledraw() {
	if (!(_doCulling) || getVisible()) {
		glBindVertexArray(_VAO);  // bind and draw cube
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
}

void cube::_draw(uint32_t rendermethod) {

	if (!(_doCulling) || getVisible()) {
		glBindVertexArray(_VAO);  // bind and draw cube

		glDrawElements(rendermethod, 36, GL_UNSIGNED_INT, 0);
	}
};

void cube::update(float dt) {
	//move(glm::vec3(4.f * dt, 0.f, 0.f));
	//_matrix = glm::translate(_matrix, glm::vec3(2.f * dt, 0.f, 0.f));
	rotate(glm::radians(45.f * dt), glm::vec3(0.5f, 0.5f, 0.5f));
	//_matrix = glm::rotate(_matrix, glm::radians(45.f * dt), glm::vec3(0.f, 1.f, 0.f));
};
#pragma endregion

#pragma region Plane Definitions


void plane::addTan(std::vector<float>& outvec, float* fin, unsigned flen, unsigned* iin, unsigned ilen) {

	outvec.shrink_to_fit();
	// 192 in div by 8 to remove stride for vertexes then times by 11 for new stride = 264
	// assign is used instead of reserve as we need the vector to be pre populated to memmove to it
	outvec.assign(264,0.f);
	std::vector<vertex> temp;
	temp.reserve(flen / 8);
	vertex V;
	V.tan = glm::vec3{ 0.f };
	for (unsigned i = 0; i < flen; i += 8) {
		V.pos = glm::vec3{ fin[i + 0], fin[i + 1], fin[i + 2] };
		V.norm = glm::vec3{ fin[i + 3], fin[i + 4], fin[i + 5] };
		V.uv = glm::vec2{ fin[i + 6], fin[i + 7] };
		temp.push_back(V);
	}
	//loop the vertexes, add tangents accordingly
	for (unsigned i = 0; i < ilen; i += 3) {
		vertex& v1 = temp.at(iin[i + 0]);
		vertex& v2 = temp.at(iin[i + 1]);
		vertex& v3 = temp.at(iin[i + 2]);
		glm::vec3 dp1 = v2.pos - v1.pos;
		glm::vec3 dp2 = v3.pos - v1.pos;

		glm::vec2 uv1 = v2.uv - v1.uv;
		glm::vec2 uv2 = v3.uv - v1.uv;

		float r = 1.f / ((uv1.x * uv2.y) - (uv1.y * uv2.x));
		//set tangent
		glm::vec3 tan = r * ((dp1 * uv2.y) - (dp2 * uv1.y));
		v1.tan += tan;
		v2.tan += tan;
		v3.tan += tan;
	}
	//normalize any vertexes that had multiple tangents added, then push all the data back to the outvec
	for (vertex& v : temp) {
		v.tan = glm::normalize(v.tan);
		outvec.push_back(v.pos.x);
		outvec.push_back(v.pos.y);
		outvec.push_back(v.pos.z);
		outvec.push_back(v.norm.x);
		outvec.push_back(v.norm.y);
		outvec.push_back(v.norm.z);
		outvec.push_back(v.uv.x);
		outvec.push_back(v.uv.y);
		outvec.push_back(v.tan.x);
		outvec.push_back(v.tan.y);
		outvec.push_back(v.tan.z);
	}

	//DOCUMENT
	//works only bc both vectors, and all of the contents of vertex are all just contiguous floats underneath
	//don't use memcpy, crashes sometimes. memmove_s has never crashed and is still an order of magnitude faster
	memmove_s(outvec.data(), outvec.size() * sizeof(float), temp.data(), temp.size() * sizeof(vertex));
	

}


plane::plane(const glm::vec2& size, const glm::vec3& pos, float t) {
	_aabb = { glm::vec3(size.x, 0.001f, size.y), -glm::vec3(size.x, 0.001f, size.y) };
	float temp[32] = { 
	//              XZ sizes               normals      UV coords
	//      <---------------------->  <------------->  <--------->
			-size.x, 0.0f,  -size.y, 0.0f, 1.0f, 0.0f, 0.0f      , 0.0f      ,
			 size.x, 0.0f,  -size.y, 0.0f, 1.0f, 0.0f, 0.0f      , size.y * t,
			 size.x, 0.0f,   size.y, 0.0f, 1.0f, 0.0f, size.x * t, size.y * t,
			-size.x, 0.0f,   size.y, 0.0f, 1.0f, 0.0f, size.x * t, 0.0f      ,
	};
	unsigned int indices[6] = {
		3,2,1,
		3,1,0
	};

	std::vector<float> myvertexinfo{};

	addTan(myvertexinfo, temp, 32, indices, 6);

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, myvertexinfo.size() * sizeof(float), myvertexinfo.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// uv attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// uv attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	move(pos);
}

void plane::_simpledraw() {
	if (!(_doCulling) || getVisible()) {
		glBindVertexArray(_VAO);  // bind and draw floor
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void plane::_draw(uint32_t rendermethod) {
	if (!(_doCulling) || getVisible()) {
		glBindVertexArray(_VAO);  // bind and draw floor
		glDrawElements(rendermethod, 6, GL_UNSIGNED_INT, 0);
	}
}


void plane::update(float dt) {};

#pragma endregion