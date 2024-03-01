#pragma once
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <rapidjson/include/rapidjson/rapidjson.h>
#include <rapidjson/include/rapidjson/document.h>
#include <rapidjson/include/rapidjson/stringbuffer.h>	
#include <rapidjson/include/rapidjson/prettywriter.h>
#include <string>
#include <fstream>
#include <sstream>

namespace My_Ezz
{
	class ShaderProgram;
	class VertexBuffer;
	class VertexArray;
	class IndexBuffer;
	class Texture2D;


	class IDrawing
	{
	public:
		IDrawing() {}
		virtual ~IDrawing() {}
		virtual void LoadMesh(std::shared_ptr<VertexBuffer> m_pPositions, std::shared_ptr<IndexBuffer> m_pIndexes) = 0;
		virtual void Draw(std::shared_ptr<ShaderProgram> pShaderProgram, bool bSelected = false) = 0;
		virtual void SetShaderMatrixs(const glm::mat4& mProjectionMatrix, const glm::mat4& mViewMatrix) = 0;
	};

	class ITransforming
	{
	public:
		ITransforming() {}
		virtual ~ITransforming() {}
		virtual void Rotate(const glm::vec3& vRotation) = 0;
		virtual void Translate(const glm::vec3& vTraslate) = 0;
		virtual void Scale(const double& dScale) = 0;
		virtual void SetPosition(const glm::vec3& vPosition) = 0;
		virtual void SetRotation(const glm::vec3& vRotation) = 0;
		virtual void SetScale(const double& dScale) = 0;
	};

	class ISerializable
	{
	public:
		ISerializable() {}
		virtual ~ISerializable() {}
		bool DeserializeFromFile(const std::string& filePath)
		{
			std::ifstream f(filePath);
			std::stringstream buffer;
			buffer << f.rdbuf();
			f.close();

			return Deserialize(buffer.str());
		}
		bool SerializeToFile(const std::string& filePath)
		{
			std::ofstream f(filePath);
			std::string s = Serialize();
			f << s;
			f.flush();
			f.close();

			return true;
		}

		virtual std::string Serialize() const final
		{
			rapidjson::StringBuffer ss;
			rapidjson::Writer<rapidjson::StringBuffer> writer(ss);		// Can also use Writer for condensed formatting
			if (Save(&writer) && SaveRefs(&writer))
				return ss.GetString();
			return "";
		}
		virtual bool Deserialize(const std::string& s) final
		{
			rapidjson::Document doc;
			if (!InitDocument(s, doc))
				return false;

			Load(doc);

			return true;
		}
		virtual bool Load(const rapidjson::Value& obj) = 0;
		virtual bool Save(rapidjson::Writer<rapidjson::StringBuffer>* writer) const = 0;
		virtual bool SaveRefs(rapidjson::Writer<rapidjson::StringBuffer>* writer) const { return true; };
	protected:
		bool InitDocument(const std::string& s, rapidjson::Document& doc)
		{
			if (s.empty())
				return false;

			std::string validJson(s);

			return !doc.Parse(validJson.c_str()).HasParseError() ? true : false;
		}
	};

}
