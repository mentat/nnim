#pragma warning(disable:4786)
#include <string>
#include <iostream>
#include <fstream>

// Set this to non-zero to disable logging.
#define NO_KIM_LOG 0

#include "log.h"


using namespace std;

#ifdef NDEBUG
static  ofstream out("/dev/null");
#elif defined WIN32
static ofstream out("c:\\log.txt");
#else
#define USE_COUT
#endif

ostream &debug()
{
#ifdef USE_COUT
  return cout;
#else
  return out;
#endif
}

ostream &debug(int priority)
{
  return debug();
}
ostream &critical()
{
  return debug();
}
ostream &error()
{
  return debug();
}

/*
kLog::kLog(const string& msg)
{

  if (NO_KIM_LOG != 0)
    return;

#if defined(__WXMSW__) || defined(__WXGTK__)
  wxLogMessage(msg.c_str());
#else
  cout << msg << endl;
#endif

}

 
   #include <iostream>
     #include <iomanip>
       #include <algorithm>
         #include <iterator>

	   class fixedwidth
	     {
	         const char *_s;

		   public:
		       fixedwidth(const char *s)
		             : _s(s) {}

			         friend ostream &
				     operator<< (ostream &o, const fixedwidth &fw);
				       };

  ostream &
    operator<< (ostream &o, const fixedwidth &fw)
	  {
		      ostream_iterator<char> iter(o);
		          size_t w = o.width(); o.width(0);
			      size_t l = char_traits<char>::length(fw._s);
			          if (o.flags() & (ostream::right | ostream::internal))
					        fill_n(iter, w - l, ' ');
				      copy(fw._s, fw._s + min<size_t>(l, w), iter);
				          if (o.flags() & ostream::left)
						        fill_n(iter, w - l, ' ');
					      return o;
					        }

 
 
 */
