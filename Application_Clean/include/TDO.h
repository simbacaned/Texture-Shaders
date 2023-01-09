#pragma once
#include <vector>

#pragma warning(push, 0)
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glad/glad.h>

#pragma warning(pop)

class TDO {
protected:
	struct vertex {
		glm::vec3 pos;
		glm::vec3 norm;
		glm::vec2 uv;
		glm::vec3 tan;
	};
	struct AABB {
		glm::vec3 min;
		glm::vec3 max;
		AABB operator* (const glm::vec3& rhs) const {
			return AABB{min * rhs, max * rhs};
		}
	}_aabb;
	glm::mat4 _matrix { 1.f };
	glm::mat4 _matrixS{ 1.f };

	glm::vec3 _scale{ 1.f };
	float _uvScale{ 1.f };

	bool _isOffscreen = false;
	bool _isOccluded = false;
	bool _doCulling = true;
	
	glm::vec3 _color{ glm::vec3(1.0)};
	unsigned _VAO = NULL;
	unsigned _VBO = NULL;
	unsigned _EBO = NULL;
	void updateMatrix();
public:
	TDO() = default;
	virtual void _draw(uint32_t rendermethod) = 0;
	virtual void _simpledraw() = 0;
	virtual void update(float dt) = 0;
	virtual void addTan(std::vector<float>& outvec, float* fin, unsigned flen, unsigned* iin, unsigned ilen) = 0;
	void setColor  (glm::vec3 in);
	void setPos	   (const glm::vec3& in);
	void setScale  (const glm::vec3& scale);
	void setScale  (float scale);
	void setIsOffscreen(bool in) { _isOffscreen = in; };
	void setIsOccluded (bool in) { _isOccluded  = in; };
	void setDoCull (bool in) { _doCulling = in; };
	
	void move  (const glm::vec3& in);
	void rotate(float speed, const glm::vec3& axis);

	_NODISCARD inline bool      getIsOffscreen()       { return _isOffscreen   ;};
	_NODISCARD inline bool      getIsOffscreen() const { return _isOffscreen   ;};
	_NODISCARD inline bool      getVisible    ()       { return !(_isOffscreen || _isOccluded)     ;};
	_NODISCARD inline bool      getVisible    () const { return !(_isOffscreen || _isOccluded)     ;};
	_NODISCARD inline bool      getDoCull     ()       { return _doCulling     ;};
	_NODISCARD inline bool      getDoCull     () const { return _doCulling     ;};
	_NODISCARD inline AABB      getAABB       ()       { return _aabb * _scale ;};
	_NODISCARD inline AABB      getAABB       () const { return _aabb * _scale ;};
	_NODISCARD inline glm::vec3 getScale      ()       { return _scale         ;};
	_NODISCARD inline glm::vec3 getScale      () const { return _scale         ;};
	_NODISCARD inline glm::vec3 getPos        ()       { return _matrix[3]     ;};
	_NODISCARD inline glm::vec3 getPos        () const { return _matrix[3]     ;};
	_NODISCARD inline glm::mat4 getMatrix     ()       { return _matrixS       ;};
	_NODISCARD inline glm::mat4 getMatrix     () const { return _matrixS       ;};
	_NODISCARD inline glm::vec3 getColor      ()       { return _color         ;};
	_NODISCARD inline glm::vec3 getColor      () const { return _color         ;};
	
	
};

class cube : public TDO{
private: 

public:
	cube(const glm::vec3& size = glm::vec3{1.f}, const glm::vec3& pos = glm::vec3{ 0.f }, float t = 1.f);
	void _draw(uint32_t rendermethod) override;
	void _simpledraw() override;
	void update(float dt) override;
	void addTan(std::vector<float>& outvec,float* fin, unsigned flen, unsigned* iin, unsigned ilen) override;

};

class plane : public TDO{
private:

public:
	plane(const glm::vec2& size = glm::vec2{1.f}, const glm::vec3& pos = glm::vec3{ 0.f }, float t = 1.f);
	void _draw(uint32_t rendermethod) override;
	void _simpledraw() override;
	void update(float dt) override;
	void addTan(std::vector<float>& outvec,float* fin, unsigned flen, unsigned* iin, unsigned ilen) override;

};
