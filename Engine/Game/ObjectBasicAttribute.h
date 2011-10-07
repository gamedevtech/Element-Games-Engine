#ifndef EG_GAME_OBJECT_BASIC_ATTRIBUTE_H
#define EG_GAME_OBJECT_BASIC_ATTRIBUTE_H

#include <string>

#include "../Math/Math.h"
#include "ObjectAttribute.h"

namespace EG{
	namespace Game{
		class ObjectAttributeBasicTransformation : public ObjectAttribute{
		public:
			ObjectAttributeBasicTransformation(glm::mat4 _transformation);
			~ObjectAttributeBasicTransformation(void);

			void SetTransformation(glm::mat4 _transformation);
			glm::mat4 GetTransformation(void);
		private:
			glm::mat4 transformation;
		};

		class ObjectAttributeBasicString : public ObjectAttribute{
		public:
			ObjectAttributeBasicString(std::string _key, std::string _value);
			~ObjectAttributeBasicString(void);

			void SetKey(std::string _key);
			void SetValue(std::string _value);

			std::string GetKey(void);
			std::string GetValue(void);
		private:
			std::string key, value;
		};

		class ObjectAttributeBasicInteger : public ObjectAttribute{
		public:
			ObjectAttributeBasicInteger(std::string _key, int _value);
			~ObjectAttributeBasicInteger(void);

			void SetKey(std::string _key);
			void SetValue(int _value);

			std::string GetKey(void);
			int GetValue(void);
		private:
			std::string key;
			int value;
		};

		class ObjectAttributeBasicFloat : public ObjectAttribute{
		public:
			ObjectAttributeBasicFloat(std::string _key, float _value);
			~ObjectAttributeBasicFloat(void);

			void SetKey(std::string _key);
			void SetValue(float _value);

			std::string GetKey(void);
			float GetValue(void);
		private:
			std::string key;
			float value;
		};
	}
}

#endif
