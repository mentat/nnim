#include "wx/wx.h"
#include "wx/treectrl.h"
#include "wx/hash.h"
#include "wx/file.h"
#include "wx/dynarray.h"

#include "NAutoTxt.h"

#include <fstream>


class BST
{
public:
	BST(const wxString& item);
	~BST();

	void Add(const wxString& item);
	wxString GetClosest(const wxString& item);

private:

	wxString m_data;
	BST * m_link1;
	BST * m_link2;
};

BST::BST(const wxString& item)
{
	m_data = item;
	m_link1 = NULL;
	m_link2 = NULL;
}

BST::~BST()
{
	delete m_link1;
	delete m_link2;
}

void BST::Add(const wxString& item)
{
	int result(m_data.CmpNoCase(item));

	if (result == 0)
		return;

	if (result < 0)
		if (m_link1)
			m_link1->Add(item);
		else 
			m_link1 = new BST(item);
	else
		if (m_link2)
			m_link2->Add(item);
		else
			m_link2 = new BST(item);

}

wxString BST::GetClosest(const wxString &item)
{
	int result(m_data.CmpNoCase(item));

	if (result < 0)
	{

		wxLogDebug(wxT("less than"));
		if (m_link1)
			return m_link1->GetClosest(item);
		else
			return "";

	}

	if (result == 0)
	{
		wxLogDebug(wxT("equal"));
		return m_data;
	}

	wxString tmp;
	if (m_data.Lower().StartsWith(item))
	{
		wxLogDebug(wxT("Starts with"));
		if (m_link2)
		{
			tmp = m_link2->GetClosest(item);
			if (tmp == "")
				return m_data;
		}
		else
			return m_data;
	}
	else
		if (m_link2)
			return m_link2->GetClosest(item);

	return "";

}




BEGIN_EVENT_TABLE(wxAutoTextCtrl, wxTextCtrl)
EVT_CHAR(wxAutoTextCtrl::OnChar)
END_EVENT_TABLE()

using namespace std;
wxAutoTextCtrl::wxAutoTextCtrl()
{
	wxTextCtrl::Init();
	//m_table = new wxHashTable(wxKEY_STRING, 32000);

}

wxAutoTextCtrl::wxAutoTextCtrl(wxWindow *parent, wxWindowID id,
               const wxString& value,
               const wxPoint& pos,
               const wxSize& size,
               long style,
               const wxValidator& validator,
               const wxString& name)
			   : wxTextCtrl(parent, id, value, pos, size, style, validator, name)
{
	m_table = NULL;// new wxHashTable(wxKEY_STRING, 106000);
	//m_bst = NULL;
	ReadDict();

}


bool wxAutoTextCtrl::Create(wxWindow *parent, wxWindowID id,
                const wxString& value,
                const wxPoint& pos,
                const wxSize& size,
                long style,
                const wxValidator& validator,
				const wxString& name)
{
	//m_table = new wxHashTable(wxKEY_STRING, 32000);


	return wxTextCtrl::Create(parent, id, value, pos, size, style, validator, name);
}

wxAutoTextCtrl::~wxAutoTextCtrl()
{
	//int x = x + 10;
//	delete m_bst;
}

void wxAutoTextCtrl::ReadDict()
{
	//wxArrayString stringArray;
	/*wxFile dict;
	wxString word;
	wxASSERT(dict.Open(wxT("eng.dict")));

	wxArrayString stringArray;

	char ch;

	while(!dict.Eof() && !dict.Error())
	{
		off_t rd = dict.Read((void *)ch, 1);
		if (rd == wxInvalidOffset)
			break;

		// sorry, this is only ASCII english
		if ((ch >= 'a') || (ch <= 'z') || (ch >= 'A') || (ch <= 'Z') || (ch == ' '))
			word+=ch;
		else
			if (word.length() > 0)
				stringArray.Add(word);

		word.Clear();
	}*/

	ifstream in("eng.dict");

	char line[80];

	wxLogDebug(wxT("Creating BST"));
	//in.getline(line, 80, '\n');
	wxString word;
	/*if (word.Trim(TRUE) != "")
		//m_bst = new BST(word.Trim(TRUE));
		stringArray.Add(word.Trim(TRUE));	
	else
	{
		wxLogDebug(wxT("First item is null..."));
		return;
	}*/

	while (in)
	{
		in.getline(line, 80, '\n');
		word = line;
		if (word.Trim(TRUE) != "")
			//m_bst->Add(word.Trim(TRUE));
			stringArray.Add(word.Trim(TRUE));
	}

	wxLogDebug(wxT("Sorting..."));
	stringArray.Sort(TRUE);
	wxLogDebug(wxT("Sorting done..."));

	m_table = new wxHashTable(wxKEY_STRING, stringArray.GetCount() * 4);

	wxLogDebug(wxT("BST Done"));

	unsigned long int arrayIndex(0);
	unsigned int hashLength(1);
	unsigned long int arrayCount(stringArray.GetCount());

	wxString log;
	log.Printf(wxT("Array size is: %u"), arrayCount);
	wxLogDebug(log);

	wxString curHash;
	bool working(true);

	unsigned long int totalCount(0);

	wxLogDebug(wxT("Generating hash table..."));
	while (true)
	{
		if (arrayIndex == (arrayCount -1))
			if (curHash == stringArray.Last())
				break;

		if (hashLength > stringArray.Item(arrayIndex).Length())
		{
			hashLength = 1;
			arrayIndex++;

			if (arrayIndex == arrayCount)
				break;
		}

		wxString hash(stringArray.Item(arrayIndex).Mid(0, hashLength).Lower());

		bool otherWithSameHash(true);

		
		for (unsigned long int i = arrayIndex + 1; ((i < arrayCount) && (!otherWithSameHash)); i++)
		{	
			wxString tmp(stringArray.Item(i).Mid(0, hashLength).Lower());
			if (tmp == hash)
				otherWithSameHash = true;
		}
		
		if (otherWithSameHash)
			if (m_table->Get(hash) == (wxObject *)NULL)
			{
				if ((arrayIndex % 1000) == 0)  wxLogDebug(wxString(wxT("putting: ")) + hash + stringArray.Item(arrayIndex));
				totalCount++;
				m_table->Put(hash, (wxObject *)&stringArray.Item(arrayIndex));
			}

		hashLength++;

	}
	wxLogDebug(wxT("Hash table complete."));
	wxString loged;
	loged.Printf(wxT("Total size is %u"), totalCount);
	wxLogDebug(loged);

}

void wxAutoTextCtrl::OnChar(wxKeyEvent &event)
{
	int keyPressed(event.GetKeyCode());

	if (keyPressed == WXK_SPACE)
		m_curWord = "";
	else
		if (keyPressed < 127)
		m_curWord += wxChar(keyPressed);
		else 
			if (keyPressed == 127) 
				if (!m_curWord.IsEmpty()) 
					m_curWord = m_curWord.Mid(0, m_curWord.length() - 1);

	wxTextCtrl::OnChar(event);

	// now try to find in hash table

//	if (m_bst)
//		m_curFoundWord = m_bst->GetClosest(m_curWord.Lower());

	wxLogDebug(m_curFoundWord);

	//DisplayClosest();

}

void wxAutoTextCtrl::DisplayClosest()
{

	long int closestLen(m_curFoundWord.Length());
	long int curLen(m_curWord.length());

	if (closestLen == 0) return;

	long int sel = GetInsertionPoint();
	WriteText(m_curFoundWord.Mid(curLen - 1, closestLen-curLen));

	SetSelection(sel, sel + (closestLen - curLen));

}
