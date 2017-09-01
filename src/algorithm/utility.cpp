

//calculate two bytes CRC
int calcCRC(const char * crcStartPtr, int  buffLen)
{
	int crc = 0xFFFF;
	char * bufferTop = crcStartPtr + buffLen;
	char * crcBuffPtr = crcStartPtr; 
	int  i;

	while (crcBuffPtr < bufferTop)
	{
		crc = (int)(crc & 0xff00) + (*(char *)crcBuffPtr ^ LOBYTE(crc));
		for (i = 0; i<8; i++)
		{
			if ((crc & 0x0001) == TRUE)
				crc = (crc >> 1) ^ 0xA001;
			else
				crc = (crc >> 1);
		}
		crcBuffPtr++;
	}
	return (crc);
} 

//open and parse XML file
void initFromFile(QString filename)
{
	QDomDocument doc;
	int errrow,errcol;
	QString errstr;
	QFile file(filename);
	if (!file.open(QFile::ReadOnly | QFile::Text))	{
		return;
	}
	if (!doc.setContent(&file, true, &errstr, &errrow, &errcol))	{
		return;
	}
	QDomElement root = doc.documentElement();
	if (root.tagName() != "TransModbusProtocol")
		return;
	QDomNode node = root.firstChild();
	while (!node.isNull()) {		
		parseEntry(node.toElement());
		node = node.nextSibling();
	}
}

void parseEntry(QDomElement element)
{
	QDomNode node = element.firstChild();	
	while (!node.isNull()) 
	{
		QString tagname = node.toElement().tagName();
		
		if (node.toElement().tagName() == "LocalAddr") 
		{		
			if (node.toElement().hasAttribute("addr")) {	//<NVR name="QT">	
				m_localAddr = node.toElement().attributeNode("addr").value().toUInt();
			}			
		}
		else if (node.toElement().tagName() == "RemotAddr")
		{
			if (node.toElement().hasAttribute("addr")) {		
				m_remoteAddr = node.toElement().attributeNode("addr").value().toUInt();
			}
		}
		else if (node.toElement().tagName() == "StartAddr")
		{
			if (node.toElement().hasAttribute("addr")) {		
				/*x=*/node.toElement().attributeNode("addr").value();
			}
			if (node.toElement().hasAttribute("dataCnt")) {	
				/*x=*/node.toElement().attributeNode("dataCnt").value();
			}
			if (node.toElement().hasAttribute("ptid")) {
				/*x=*/node.toElement().attributeNode("ptid").value();
			}
		}
		else if (node.toElement().tagName() == "EndAddr")
		{
			if (node.toElement().hasAttribute("addr")) {
				/*x=*/node.toElement().attributeNode("addr").value();
			}
		}

		node = node.nextSibling();
	}
}