//#include "SpineTextureLoader.h"
//#include <spine.h>
//#include <Extension.h>
//#include <SGAMEDEVICE.h>
//#include <SGAMEDIR.h>
//#include <STEX.h>
//
//std::vector<void *> MySpineExtension::m_DefaultInstance;
//int MySpineExtension::SpineObjectIndex = 0;
//SpineTextureLoader::SpineTextureLoader() : m_Tex(new GameTexture())
//{
//}
//
//
//SpineTextureLoader::~SpineTextureLoader()
//{
//
//}
//
//void SpineTextureLoader::load(spine::AtlasPage& page, const spine::String& path)
//{
//	char temp[255];
//	const char* tempPtr = path.buffer();
//	memset(temp, '\0', 255);
//	memcpy_s(temp, path.length(), tempPtr, path.length());
//	GameTexture::Load(temp);
//	spine::String NewString = path;
//	spine::Format tempd = page.format;
//	page.setRendererObject((void*)&m_Tex);
//	void* tempp = page.getRendererObject();
//}
//
//void SpineTextureLoader::unload(void* texture)
//{
//
//}
//
//MySpineExtension::MySpineExtension()
//{
//}
//
//
//MySpineExtension::~MySpineExtension()
//{
//	for (auto& item : m_DefaultInstance)
//	{
//		if (item != nullptr)
//		{
//			delete item;
//			item = nullptr;
//		}
//	}
//}
////
////void *MySpineExtension::_alloc(size_t size, const char *file, int line) 
////{
////	return nullptr;
////}
////
////void *MySpineExtension::_calloc(size_t size, const char *file, int line) 
////{
////	return nullptr;
////}
////
////void *MySpineExtension::_realloc(void *ptr, size_t size, const char *file, int line) 
////{
////	return nullptr;
////}
////
/////// If you provide a spineAllocFunc, you should also provide a spineFreeFunc
////void MySpineExtension::_free(void *mem, const char *file, int line) 
////{
////	delete this;
////}
////
////char * MySpineExtension::_readFile(const spine::String &path, int *length)
////{
////	char temp = 'a';
////	return &temp;
////}
////
////spine::SpineExtension* spine::getDefaultExtension()
////{
////	return MySpineExtension::DefaultExtention();
////}