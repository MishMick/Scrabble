#include "xml.h"

void loadXmlDocumentFromPath(pugi::xml_document &document, const std::string &path)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(path.c_str());    
    cocos2d::Data pData = FileUtils::getInstance()->getDataFromFile(fullPath.c_str());
    document.load_buffer(pData.getBytes(), pData.getSize());    
}
