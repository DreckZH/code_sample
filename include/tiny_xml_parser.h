#ifndef XMLCONFIGPTR
#define XMLCONFIGPTR

#ifdef DLL_BUILD
#  define DLL_EXPORT Q_DECL_EXPORT
#else
#  define DLL_EXPORT Q_DECL_IMPORT
#endif

#include <QMap>
#include <QDir>

//typedef QMap<QString, QString> ElementAttributes;
//typedef QList<XMLNode*> Elements;
//typedef QMap<QString, QMap<QString, QString> > TopLIDElemsAttributes_EX;
//typedef QMap<QString, QMap<QString, QString> > TopLIDElemsAttributes;

//class XmlConfigPtr;
typedef char* (/*XmlConfigPtr::*/*pchaAttriFun)();

enum ErrorNo {
	OK,
	NON_FILE,
	FILEPATH_ERROR,
	FILELOAD_ERROR,
	PARSE_ERROR,
	UNVALID_ATTRPATH,
    ELEMENT_ERROR
};
static	QString Erro[] = {
	"OK",
	"NON_FILE",
	"FILEPATH_ERROR",
	"FILELOAD_ERROR",
	"PARSE_ERROR",
	"UNVALID_ATTRPATH",
	"ELEMENT_ERROR"
};

#define TR2CHAR(x)		(x.toLocal8Bit().data())
#define TR2QSTRING(x)	(QString::fromLocal8Bit(x))

namespace tinyxml2{
	class XMLNode;
	class TYXMLDocument;
}
using namespace tinyxml2;

class DLL_EXPORT XmlConfigPtr
{
public:
    explicit XmlConfigPtr();
    //XmlConfigPtr(const QString path);
    virtual ~XmlConfigPtr();

	//virtual char *getAllChildLevelAttrs(/*QString elename,*/ pchaAttriFun pf) = 0;

    /*load xml file,unload xml file,release */
    int loadFile(const QString &file);
    void unLoadFile();

   /*get one element which path is tags,parameter:elem is m_doc now as absolute path,
    change later if needed 	*/
	virtual XMLNode* getElement(QStringList &tags, XMLNode* elem );
	/*get all chidren elements of depth*/
	virtual QList<XMLNode*> getChildrenElemsEx(const QString &ref, int &depth);
	//get all subelements of ref
	virtual QList<XMLNode*> getChildrenElems(const QString &ref);

    /*get attribute value of ref if type=0, or get section text of one element if type=1*/
	virtual QString getContent(const QString &ref, const int type = 0);

    /*get one element's all attributes stored in ElementAttributes as return value,
    key_path:element's absolute path*/
	virtual QMap<QString, QString> getElemAttributes(QStringList key_path);

    /*return a map contains all attributes in xml , like [project.Protocols.RcbCfg.No : 5]*/
	const QMap<QString, QMap<QString, QString> > getAllAttris() const { return m_attrs; };

	const TYXMLDocument* getXmlDom() const { return m_doc; };

protected:
    /*parse xml,get all elements' attributes into local memory,
    orgnized as type of QMap<QString, ElementAttributes> */
	int parseCfg(const XMLNode* node);

	QString getChildrenText(QStringList &tags/*, XMLNode* elem = m_doc*/);
	/*get one element's one attribute tags:absolute path of the attribute */
	QString getAttribute(QStringList &tags/*, XMLNode* elem = m_doc*/);

private:
	static QStringList KeyList;

protected:
    TYXMLDocument *m_doc;
	QMap<QString, QMap<QString, QString> > m_attrs;
};

#endif // XMLCONFIGPTR

