#include "Sky.h"

Sky::Sky(Model_type model_type, glm::vec3 transform, glm::vec3 scale, std::string texture_name, float rotation_angle, glm::vec3 rotation,
	bool is_visible, bool is_rotating)
	:Basic(model_type, transform, scale, texture_name, rotation_angle, rotation, is_visible, is_rotating) {
	
}

void Sky::draw() {
	if (this->is_visible)
	{
		spLambertTextured->use();
		glm::mat4 M = glm::mat4(1.0f);
		M = glm::translate(M, transform);
		M = glm::rotate(M, glm::radians(rotation_angle), rotation);
		M = glm::scale(M, scale);
		glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M));
		glEnableVertexAttribArray(spLambertTextured->a("vertex"));
		glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, Basic::vertices[model_type]);

		glEnableVertexAttribArray(spLambertTextured->a("normal"));
		glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, Basic::normals[model_type]);

		glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
		glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, Basic::texCoords[model_type]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_handle);
		glUniform1i(sp->u("tex"), 0);
		glDrawArrays(GL_TRIANGLES, 0, Basic::vertexCount[model_type]);
		glDisableVertexAttribArray(spLambertTextured->a("vertex"));
		glDisableVertexAttribArray(spLambertTextured->a("normal"));
		glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
	}
}
