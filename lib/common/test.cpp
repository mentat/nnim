#include <xml.h>
#include <contact.h>

#define VERIFY(a,b) {if((a)==b)cout<< "Passed.." <<endl; else {cout <<"Failed at "<< __FILE__ << ":" << __LINE__ << endl;failures++;}}
int failures=0;

int testChild()
{
  cout << "------begin Testing XML Childs ------\n";
  XMLNode n;

  n.setName("hej");
  n.addChild("aloha");
  n.child("aloha").setProperty("hej","hello");

  VERIFY(n.numChildren("aloha"),1);
  VERIFY(n.child("aloha").property("hej"),"hello");
  VERIFY(n.child("doesntexist").child("notthisoneeither").name(),"");
  VERIFY(n.child("doesntexist").property("notthisoneeither"),"");
}

int testProperty()
{
  cout << "------begin Testing XML Properties ------\n";
  XMLNode n;

  n.setName("hej");

  n.setProperty("test","yes");

  VERIFY(n.property("test"),"yes");
  VERIFY(n["test"],"yes");

  VERIFY(n.property("nonexisting"),"");
}

int testParsing()
{
  cout << "------begin Testing XML Parsing ------\n";

  XMLParser e("   <messages>"
              "<message sender=\"test\" subject=\"-\">"
              "foo bla bla"
              "</message>"
              "</messages> ");
  XMLNode bla;
  XMLNode n;
  XMLNode *blargh= new XMLNode();

  *blargh = e.get_root();
  bla=*blargh;
  n=*blargh;
  delete blargh;
  blargh=0;

  n.setProperty("foo","bar");

  VERIFY(bla.property("foo"),"bar");
  VERIFY(n.name(),"messages");
  bla.setName("hej");
  VERIFY(n.name(),"hej");

}

int testParsing2()
{
  cout << "------begin Testing invalid XML Parsing ------\n";

  XMLParser e("   <messages"
              "<message sender=\"test\" subject=\"-\">"
              "foo bla bla"
              "<message>"
              "</messages> ");
  XMLNode bla;
  XMLNode n;
  XMLNode *blargh= new XMLNode();

  *blargh = e.get_root();
  bla=*blargh;
  n=*blargh;
  delete blargh;
  blargh=0;

  n.setProperty("foo","bar");
  VERIFY(bla.property("foo"),"bar");
  bla.setName("hej");
  VERIFY(n.name(),"hej");

}

int testConst()
{
  cout << "------begin Testing constant nodes ------\n";
  XMLNode a,c;
  a.setName("henrik");
  a.setProperty("foo","var");

  const XMLNode b(a);
  c=b;
  c.setName("nisse");

  VERIFY(b.name(),"henrik");
  VERIFY(b["foo"],"var");

  VERIFY(c.name(),"nisse");
  VERIFY(c["foo"],"var");
}
int testContact()
{
  Contact d;
  cout << "------begin Testing Contacts ------\n";

  Contact c;
  c.setProtocol("kit");
  c.setServerId("henrik");

  VERIFY (c.protocol(),"kit");
  VERIFY (c.serverId(),"henrik");
  VERIFY (c.nick(),"henrik");

  c.setNick("nisse");

  VERIFY (c.nick(),"nisse");
  VERIFY (c.status(),Contact::Offline);

  c.setStatus(Contact::Online);
  VERIFY (c.status(),Contact::Online);
  d=c;

  VERIFY (d.protocol(),"kit");
  VERIFY (d.serverId(),"henrik");
  VERIFY (d.nick(),"nisse");

  VERIFY ((c==d),true);

}

int testContact2()
{
  Contact c;

  VERIFY (c.protocol(),"");
  VERIFY (c.serverId(),"");
  VERIFY (c.nick(),"");

}
int main()
{
  testProperty();
  testChild();
  testParsing();
  testParsing2();
  testConst();
  testContact();
  testContact2();
  return failures;
}
