
#include "xmlconfigptr.h"
#include "../thirdparty/tinyxml2/tinyxml2.h"

QStringList	XmlConfigPtr::KeyList;

XmlConfigPtr::XmlConfigPtr() /*, m_cfgfile("")*/
{
	KeyList.clear();
	m_attrs.clear();
}

XmlConfigPtr::~XmlConfigPtr()
{
    if (m_doc){
        m_doc->ClearError();
        m_doc->Clear();
        delete m_doc;
	}
	m_doc = NULL;
	//m_cfgfile.clear();
	KeyList.clear();
	m_attrs.clear();
}

int XmlConfigPtr::loadFile(const QString &file)
{
    if (file.isEmpty())
		return NON_FILE;
    QString tfile = QDir::toNativeSeparators(file);
    m_doc = new TYXMLDocument();
    if (!m_doc || tfile.isEmpty())
		return FILEPATH_ERROR;
    m_doc->LoadFile(TR2CHAR(tfile));    
	if (!m_doc->ErrorID())	{
        return parseCfg(m_doc->RootElement());
    }
	return FILELOAD_ERROR;//PARSRESLIST
}

void XmlConfigPtr::unLoadFile()
{
    if (m_doc){
        m_doc->ClearError();
        m_doc->Clear();
        delete m_doc;
    }
    m_doc = NULL;
	KeyList.clear();
	m_attrs.clear();
}

int XmlConfigPtr::parseCfg(const XMLNode* node)
{
	if (!node)
		return ELEMENT_ERROR;

	KeyList.push_back(node->Value());
	if (node->NoChildren())	{
		m_attrs.insert(KeyList.join("."), getElemAttributes(KeyList));
		KeyList.pop_back();
	}
	else {
		m_attrs.insert(KeyList.join("."), getElemAttributes(KeyList));
		parseCfg(node->FirstChild());
		KeyList.pop_back();
	}
	parseCfg(node->NextSibling());

	return 0;
}

QMap<QString, QString> XmlConfigPtr::getElemAttributes(QStringList key_path)
{
	QMap<QString, QString> attrs;
	attrs.clear();
	QString tpath = key_path.join(".");
	XMLNode * node = getElement(key_path, m_doc);
	XMLElement* elem = NULL;
	if (node)
		elem = node->ToElement();
	if (!elem || tpath.isEmpty())
		return attrs;
	attrs.insert("itemname", TR2QSTRING(elem->Value()));
	for (const XMLAttribute* attr = elem->FirstAttribute(); attr; attr = attr->Next()) {
		QString name = TR2QSTRING(attr->Name());
		attrs.insert(name/*.toLower()*/, attr->Value());
	}
	return attrs;
}

XMLNode* XmlConfigPtr::getElement(QStringList &tags, XMLNode* elem)
{
	if ((tags.count() < 1) || (elem == NULL))
		return elem;

	return getElement(tags, elem->FirstChildElement(TR2CHAR(tags.takeFirst())));
}

QString XmlConfigPtr::getContent(const QString &ref, const int type )
{
	QString attrpath(ref), res("");
	attrpath.simplified();
	if (attrpath.isEmpty())
		return Erro[UNVALID_ATTRPATH];

	QStringList tags = attrpath.split(".");
	switch (type)
	{
	case 0://atrribute
		res = getAttribute(tags/*, m_doc*/);
		break;
	case 1:	//sub elements text
		res = getChildrenText(tags/*, m_doc*/);
		break;
	default:
		res.clear();
		break;
	}
	return res;
}

QString XmlConfigPtr::getAttribute(QStringList &tags/*, XMLNode* elem*/)
{
	QString attrname = tags.takeLast();
	/*XMLNode * node = getElement(tags, m_doc);
	XMLElement* telem = NULL;
	if (node)
		telem = node->ToElement();
	return (!telem) ? "" : telem->Attribute(TR2CHAR(attrname));*/
	if (attrname.isEmpty())
		return attrname;
	return m_attrs.value(tags.join(".")).value(attrname);
}

QString XmlConfigPtr::getChildrenText(QStringList &tags/*, XMLNode* elem*/)
{
	XMLNode * node = getElement(tags, m_doc);
	if (node->NoChildren())
		return "";
	QString sep = TR2QSTRING(node->Value());
	XMLPrinter streamer;
	m_doc->Print(&streamer);

	return "<" + sep + TR2QSTRING(streamer.CStr()).section(sep, 1, 1) + sep + ">";
}


QList<XMLNode*> XmlConfigPtr::getChildrenElemsEx(const QString &ref, int &depth)
{
	ref; depth;
	QList<XMLNode*> elems;
	return elems;
}

QList<XMLNode*> XmlConfigPtr::getChildrenElems(const QString &ref)
{
	QList<XMLNode*> elems;
	elems.clear();

	QStringList tags = ref.split(".");
	XMLNode * node = getElement(tags, m_doc);
	if (node->NoChildren())
		return elems;
	for (XMLElement* ele = node->FirstChildElement(); ele; ele = ele->NextSiblingElement())	{
		elems.push_back(ele);
	}
	return elems;
}
