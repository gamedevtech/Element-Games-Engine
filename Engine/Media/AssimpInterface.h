#ifndef EG_MEDIA_ASSIMP_INTERFACE_H
#define EG_MEDIA_ASSIMP_INTERFACE_H

#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>

namespace EG{
	namespace Media{
		class AssimpInterface{
			public:
				AssimpInterface(void);
				~AssimpInterface(void);

				bool Load(std::string file_path);
			private:
				const aiScene *scene;
		};
	}
}

#endif
