#ifndef __GUI_NETS_H__
#define __GUI_NETS_H__

class wxSizer;
#define ID_NET_EDIT_TOC_USERNAME 10036
#define ID_NET_EDIT_TOC_PASSWORD 10037
#define ID_NET_EDIT_TOC_TOCSERVER 10038
#define ID_NET_EDIT_TOC_TOCPORT 10039
#define ID_NET_EDIT_TOC_LOGINSERVER 10040
#define ID_NET_EDIT_TOC_LOGINPORT 10041
#define ID_NET_EDIT_TOC_IMPORT_BUDDY 10042
#define ID_NET_EDIT_TOC_AUTO_LOGIN 10043
wxSizer *Net_TOC( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_NET_EDIT_GNU_USERNAME 10044
#define ID_NET_EDIT_GNU_PASSWORD 10045
#define ID_NET_EDIT_GNU_HOST 10046
#define ID_NET_EDIT_GNU_PORT 10047
#define ID_NET_EDIT_GNU_AUTO_LOGIN 10048
wxSizer *Net_GNU( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_NET_EDIT_ICQ_UIN 10049
#define ID_NET_EDIT_ICQ_PASSWORD 10050
#define ID_NET_EDIT_ICQ_HOST 10051
#define ID_NET_EDIT_ICQ_PORT 10052
#define ID_NET_EDIT_ICQ_AUTO_LOGIN 10053
wxSizer *Net_ICQ( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_NET_EDIT_MSN_USERNAME 10054
#define ID_NET_EDIT_MSN_PASSWORD 10055
#define ID_NET_EDIT_MSN_HOST 10056
#define ID_NET_EDIT_MSN_PORT 10057
#define ID_NET_EDIT_MSN_AUTO_LOGIN 10058
wxSizer *Net_MSN( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#define ID_NET_EDIT_YAHOO_USERNAME 10059
#define ID_NET_EDIT_YAHOO_PASSWORD 10060
#define ID_NET_EDIT_YAHOO_DATAHOST 10061
#define ID_NET_EDIT_YAHOO_DATAPORT 10062
#define ID_NET_EDIT_YAHOO_AUTHHOST 10063
#define ID_NET_EDIT_YAHOO_AUTHPORT 10064
#define ID_NET_EDIT_YAHOO_AUTO_LOGIN 10065
wxSizer *Net_YAHOO( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

#endif