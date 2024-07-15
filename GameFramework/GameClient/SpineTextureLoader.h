//#pragma once
//#include <spine.h>
//#include <vector>
//
//class GameTexture;
//class SpineTextureLoader : public spine::TextureLoader
//{
//private:
//	GameTexture* m_Tex;
//
//public:
//	SpineTextureLoader();
//	~SpineTextureLoader();
//
//public:
//	void load(spine::AtlasPage& page, const spine::String& path) override;
//	void unload(void* texture) override;
//};
//
//class MySpineExtension : spine::SpineExtension
//{
//	//void *_alloc(size_t size, const char *file, int line) override;
//
//	// void *_calloc(size_t size, const char *file, int line) override;
//
//	// void *_realloc(void *ptr, size_t size, const char *file, int line) override;
//
//	///// If you provide a spineAllocFunc, you should also provide a spineFreeFunc
//	// void _free(void *mem, const char *file, int line) override;
//
//	 char *_readFile(const spine::String &path, int *length) override;
//	 static int SpineObjectIndex;
//public:
//	 static std::vector<void *> m_DefaultInstance;
//public:
//	MySpineExtension();
//	~MySpineExtension();
//
//
//public:
//    static spine::SpineExtension* DefaultExtention()
//	{
//		m_DefaultInstance.push_back(new spine::DefaultSpineExtension);
//		return (spine::SpineExtension*)m_DefaultInstance[SpineObjectIndex++];
//	}
//
//	static void DeleteObject()
//	{
//		for (auto& item : m_DefaultInstance)
//		{
//			if (item != nullptr)
//			{
//				delete item;
//				item = nullptr;
//			}
//		}
//	}
//};
