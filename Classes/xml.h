#ifndef NBG_EXTERNAL_XML
#define NBG_EXTERNAL_XML

#include "cocos2d.h"
#include "pugixml.hpp"

USING_NS_CC;

void loadXmlDocumentFromPath(pugi::xml_document &document, const std::string &path);

#endif