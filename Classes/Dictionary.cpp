#include "Dictionary.h"

//////////////////////////////////////////////////////////////////////////
CDictionary::CDictionary()
{
}

//////////////////////////////////////////////////////////////////////////
CDictionary::~CDictionary()
{

}

bool CDictionary::FindWord(std::string &word)
{
	if(word == "")
	{
		return false;
	}
	int firstChar = (int)word[0];

	int sI = m_LetterOffsets[firstChar].first;
	int sE = m_LetterOffsets[firstChar].end;


	std::string * str;	
	for (size_t i=sI; i<sI + (sE-sI)/2+2; i++)
	{
		str = &m_Strings[i];
		int fChar = (int)str->at(0);
		if (firstChar == fChar)
		{
			if (*str == word)return true;
		}

		str = &m_Strings[sE-1-(i-sI)];
		fChar = (int)str->at(0);
		if (firstChar == fChar)
		{
			if (*str == word)return true;
		}
	}
	return false;
}

void CDictionary::CreateBinaryDump(const std::string &path)
{
	FILE * f = fopen("xml/dictionaries/en.bin","wb");
	std::string str = FileUtils::getInstance()->getStringFromFile(path);
	char buff[50];
	int sIndex = 0;
	int eIndex = 0;
	int size = 1;

	char prevFirstChar = str[0];
	int charRangeStart = 0;
	int charRangeEnd = 0;
	m_Strings.reserve(200000);
	for (size_t i=0; i<str.size(); i++)
	{
		buff[eIndex] = str[i];
		int chr = buff[eIndex];
		
		if (chr != (int)'\r')
		{
			eIndex++;
			if (chr == (int)'\n')
			{				
				eIndex = 0;
			}
		}		
		else
		{					
			buff[eIndex] = '\0';
			std::string subStr = buff;	
			fwrite(&eIndex,sizeof(int),1,f);
			fwrite(&buff,eIndex,1,f);
			size++;		

			if (buff[0] != prevFirstChar)
			{
				SLetterOffset lo;
				lo.first = charRangeStart;
				lo.end	 = charRangeEnd+1;
				lo.ch	 = prevFirstChar;

				charRangeStart = charRangeEnd;
				m_LetterOffsets[prevFirstChar] = lo;
				prevFirstChar = buff[0];
			}
			charRangeEnd++;
		}
	}	
	fwrite(&eIndex,sizeof(int),1,f);
	fwrite(&buff,eIndex,1,f);
	fclose(f);

	SLetterOffset lo;
	lo.first = charRangeStart;
	lo.end	 = charRangeEnd+1;
	lo.ch	 = prevFirstChar;
	m_LetterOffsets[prevFirstChar] = lo;

	f = fopen("xml/dictionaries/en.size","wb");	
	fwrite(&size,sizeof(int),1,f);	
	fclose(f);

	f = fopen("xml/dictionaries/en.offsets","wb");	
	int _size = m_LetterOffsets.size();
	fwrite(&_size,sizeof(int),1,f);	

	for (std::map<char,SLetterOffset>::iterator iter = m_LetterOffsets.begin(); iter!= m_LetterOffsets.end(); iter++)
	{
		SLetterOffset * lo = &iter->second;
		fwrite(&lo->ch,sizeof(char),1,f);	
		fwrite(&lo->first,sizeof(int),1,f);	
		fwrite(&lo->end,sizeof(int),1,f);	
	}	
	fclose(f);
}


//////////////////////////////////////////////////////////////////////////
bool CDictionary::Init()
{		
	//CreateBinaryDump("xml/dictionaries/en.txt");
	//return true;

	ssize_t size;
	unsigned char * sizeData = FileUtils::getInstance()->getFileData("en.size","rb",&size);
    
	int countOfWords = 0;
	memcpy(&countOfWords,&sizeData[0],sizeof(int));
    
	delete[] sizeData;
	
	int offset = 0;
	unsigned char * offsetsData = FileUtils::getInstance()->getFileData("en.offsets","rb",&size);
    
	int countOfLetters = 0;
	memcpy(&countOfLetters,&offsetsData[0],sizeof(int)); offset += sizeof(int);	
	for (int i=0; i<countOfLetters; i++)
	{
		char ch;
		int first;
		int end;
		memcpy(&ch,&offsetsData[offset],sizeof(char)); offset += sizeof(char);
		memcpy(&first,&offsetsData[offset],sizeof(int)); offset += sizeof(int);
		memcpy(&end,&offsetsData[offset],sizeof(int)); offset += sizeof(int);
        
		SLetterOffset lo;
		lo.ch		= ch;
		lo.first	= first;
		lo.end		= end;
		m_LetterOffsets[ch] = lo;
	}
	delete[] offsetsData;
	
	
	unsigned char * data = FileUtils::getInstance()->getFileData("en.bin","rb",&size);
    
	offset = 0;
	int sz = 0;
	std::string str;
	str.reserve(50);
	char buff[50];
	m_Strings.resize(countOfWords);
	int i =0;
	while (offset < size)
	{
		memcpy(&sz,&data[offset],sizeof(int));offset+=sizeof(int);
		memcpy(&buff[0],&data[offset],sz);offset+=sz;
		buff[sz] = '\0';
		m_Strings[i] = buff;
		i++;
	}
	delete[] data;
	return true;
}