
#include "gui_nets.h"

wxSizer *Net_TOC( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 1, 0, 0 );
    item0->AddGrowableCol( 0 );

    wxStaticText *item1 = new wxStaticText( parent, ID_TEXT, "User properties:", wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item2 = new wxFlexGridSizer( 2, 0, 0 );
    item2->AddGrowableCol( 1 );

    wxStaticText *item3 = new wxStaticText( parent, ID_TEXT, "Username:", wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item3, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item4 = new wxTextCtrl( parent, ID_NET_EDIT_TOC_USERNAME, "", wxDefaultPosition, wxSize(80,-1), 0 );
    item2->Add( item4, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item5 = new wxStaticText( parent, ID_TEXT, "Password:", wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item5, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item6 = new wxTextCtrl( parent, ID_NET_EDIT_TOC_PASSWORD, "", wxDefaultPosition, wxSize(80,-1), wxTE_PASSWORD );
    item2->Add( item6, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item0->Add( item2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item7 = new wxStaticText( parent, ID_TEXT, "Server properties:", wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item8 = new wxFlexGridSizer( 4, 0, 0 );
    item8->AddGrowableCol( 1 );

    wxStaticText *item9 = new wxStaticText( parent, ID_TEXT, "TOC server:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item9, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item10 = new wxTextCtrl( parent, ID_NET_EDIT_TOC_TOCSERVER, "", wxDefaultPosition, wxSize(110,-1), 0 );
    item8->Add( item10, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item11 = new wxStaticText( parent, ID_TEXT, "Port:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item11, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item12 = new wxTextCtrl( parent, ID_NET_EDIT_TOC_TOCPORT, "", wxDefaultPosition, wxSize(60,-1), 0 );
    item8->Add( item12, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxStaticText *item13 = new wxStaticText( parent, ID_TEXT, "Login server:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item13, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item14 = new wxTextCtrl( parent, ID_NET_EDIT_TOC_LOGINSERVER, "", wxDefaultPosition, wxSize(80,-1), 0 );
    item8->Add( item14, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item15 = new wxStaticText( parent, ID_TEXT, "Port:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item15, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item16 = new wxTextCtrl( parent, ID_NET_EDIT_TOC_LOGINPORT, "", wxDefaultPosition, wxSize(60,-1), 0 );
    item8->Add( item16, 0, wxALIGN_CENTRE|wxALL, 5 );

    item0->Add( item8, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item17 = new wxFlexGridSizer( 1, 0, 0 );

    wxCheckBox *item18 = new wxCheckBox( parent, ID_NET_EDIT_TOC_IMPORT_BUDDY, "Always Import Buddy List", wxDefaultPosition, wxDefaultSize, 0 );
    item17->Add( item18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxCheckBox *item19 = new wxCheckBox( parent, ID_NET_EDIT_TOC_AUTO_LOGIN, "Auto-Login", wxDefaultPosition, wxDefaultSize, 0 );
    item17->Add( item19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item0->Add( item17, 0, wxALIGN_CENTRE|wxALL, 5 );

    if (set_sizer)
    {
        parent->SetAutoLayout( TRUE );
        parent->SetSizer( item0 );
        if (call_fit)
        {
            item0->Fit( parent );
            item0->SetSizeHints( parent );
        }
    }
    
    return item0;
}

wxSizer *Net_GNU( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 1, 0, 0 );
    item0->AddGrowableCol( 0 );

    wxStaticText *item1 = new wxStaticText( parent, ID_TEXT, "User properties:", wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item2 = new wxFlexGridSizer( 2, 0, 0 );
    item2->AddGrowableCol( 1 );

    wxStaticText *item3 = new wxStaticText( parent, ID_TEXT, "Username:", wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item3, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item4 = new wxTextCtrl( parent, ID_NET_EDIT_GNU_USERNAME, "", wxDefaultPosition, wxSize(80,-1), 0 );
    item2->Add( item4, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item5 = new wxStaticText( parent, ID_TEXT, "Password:", wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item5, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item6 = new wxTextCtrl( parent, ID_NET_EDIT_GNU_PASSWORD, "", wxDefaultPosition, wxSize(80,-1), wxTE_PASSWORD );
    item2->Add( item6, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item0->Add( item2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item7 = new wxStaticText( parent, ID_TEXT, "Server properties:", wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item8 = new wxFlexGridSizer( 4, 0, 0 );
    item8->AddGrowableCol( 1 );

    wxStaticText *item9 = new wxStaticText( parent, ID_TEXT, "Host:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item9, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item10 = new wxTextCtrl( parent, ID_NET_EDIT_GNU_HOST, "", wxDefaultPosition, wxSize(110,-1), 0 );
    item8->Add( item10, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item11 = new wxStaticText( parent, ID_TEXT, "Port:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item11, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item12 = new wxTextCtrl( parent, ID_NET_EDIT_GNU_PORT, "", wxDefaultPosition, wxSize(60,-1), 0 );
    item8->Add( item12, 0, wxALIGN_CENTRE|wxALL, 5 );

    item0->Add( item8, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item13 = new wxFlexGridSizer( 1, 0, 0 );

    wxCheckBox *item14 = new wxCheckBox( parent, ID_NET_EDIT_GNU_AUTO_LOGIN, "Auto-Login", wxDefaultPosition, wxDefaultSize, 0 );
    item13->Add( item14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item0->Add( item13, 0, wxALIGN_CENTRE|wxALL, 5 );

    if (set_sizer)
    {
        parent->SetAutoLayout( TRUE );
        parent->SetSizer( item0 );
        if (call_fit)
        {
            item0->Fit( parent );
            item0->SetSizeHints( parent );
        }
    }
    
    return item0;
}

wxSizer *Net_ICQ( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 1, 0, 0 );
    item0->AddGrowableCol( 0 );

    wxStaticText *item1 = new wxStaticText( parent, ID_TEXT, "User properties:", wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item2 = new wxFlexGridSizer( 2, 0, 0 );
    item2->AddGrowableCol( 1 );

    wxStaticText *item3 = new wxStaticText( parent, ID_TEXT, "UIN:", wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item3, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item4 = new wxTextCtrl( parent, ID_NET_EDIT_ICQ_UIN, "", wxDefaultPosition, wxSize(80,-1), 0 );
    item2->Add( item4, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item5 = new wxStaticText( parent, ID_TEXT, "Password:", wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item5, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item6 = new wxTextCtrl( parent, ID_NET_EDIT_ICQ_PASSWORD, "", wxDefaultPosition, wxSize(80,-1), wxTE_PASSWORD );
    item2->Add( item6, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item0->Add( item2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item7 = new wxStaticText( parent, ID_TEXT, "Server properties:", wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item8 = new wxFlexGridSizer( 4, 0, 0 );
    item8->AddGrowableCol( 1 );

    wxStaticText *item9 = new wxStaticText( parent, ID_TEXT, "Host:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item9, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item10 = new wxTextCtrl( parent, ID_NET_EDIT_ICQ_HOST, "", wxDefaultPosition, wxSize(110,-1), 0 );
    item8->Add( item10, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item11 = new wxStaticText( parent, ID_TEXT, "Port:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item11, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item12 = new wxTextCtrl( parent, ID_NET_EDIT_ICQ_PORT, "", wxDefaultPosition, wxSize(60,-1), 0 );
    item8->Add( item12, 0, wxALIGN_CENTRE|wxALL, 5 );

    item0->Add( item8, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item13 = new wxFlexGridSizer( 1, 0, 0 );

    wxCheckBox *item14 = new wxCheckBox( parent, ID_NET_EDIT_ICQ_AUTO_LOGIN, "Auto-Login", wxDefaultPosition, wxDefaultSize, 0 );
    item13->Add( item14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item0->Add( item13, 0, wxALIGN_CENTRE|wxALL, 5 );

    if (set_sizer)
    {
        parent->SetAutoLayout( TRUE );
        parent->SetSizer( item0 );
        if (call_fit)
        {
            item0->Fit( parent );
            item0->SetSizeHints( parent );
        }
    }
    
    return item0;
}

wxSizer *Net_MSN( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 1, 0, 0 );
    item0->AddGrowableCol( 0 );

    wxStaticText *item1 = new wxStaticText( parent, ID_TEXT, "User properties:", wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item2 = new wxFlexGridSizer( 2, 0, 0 );
    item2->AddGrowableCol( 1 );

    wxStaticText *item3 = new wxStaticText( parent, ID_TEXT, "Username:", wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item3, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item4 = new wxTextCtrl( parent, ID_NET_EDIT_MSN_USERNAME, "", wxDefaultPosition, wxSize(80,-1), 0 );
    item2->Add( item4, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item5 = new wxStaticText( parent, ID_TEXT, "Password:", wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item5, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item6 = new wxTextCtrl( parent, ID_NET_EDIT_MSN_PASSWORD, "", wxDefaultPosition, wxSize(80,-1), wxTE_PASSWORD );
    item2->Add( item6, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item0->Add( item2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item7 = new wxStaticText( parent, ID_TEXT, "Server properties:", wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item8 = new wxFlexGridSizer( 4, 0, 0 );
    item8->AddGrowableCol( 1 );

    wxStaticText *item9 = new wxStaticText( parent, ID_TEXT, "Host:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item9, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item10 = new wxTextCtrl( parent, ID_NET_EDIT_MSN_HOST, "", wxDefaultPosition, wxSize(120,-1), 0 );
    item8->Add( item10, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item11 = new wxStaticText( parent, ID_TEXT, "Port:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item11, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item12 = new wxTextCtrl( parent, ID_NET_EDIT_MSN_PORT, "", wxDefaultPosition, wxSize(50,-1), 0 );
    item8->Add( item12, 0, wxALIGN_CENTRE|wxALL, 5 );

    item0->Add( item8, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item13 = new wxFlexGridSizer( 1, 0, 0 );

    wxCheckBox *item14 = new wxCheckBox( parent, ID_NET_EDIT_MSN_AUTO_LOGIN, "Auto-Login", wxDefaultPosition, wxDefaultSize, 0 );
    item13->Add( item14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item0->Add( item13, 0, wxALIGN_CENTRE|wxALL, 5 );

    if (set_sizer)
    {
        parent->SetAutoLayout( TRUE );
        parent->SetSizer( item0 );
        if (call_fit)
        {
            item0->Fit( parent );
            item0->SetSizeHints( parent );
        }
    }
    
    return item0;
}

wxSizer *Net_YAHOO( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 1, 0, 0 );
    item0->AddGrowableCol( 0 );

    wxStaticText *item1 = new wxStaticText( parent, ID_TEXT, "User properties:", wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item2 = new wxFlexGridSizer( 2, 0, 0 );
    item2->AddGrowableCol( 1 );

    wxStaticText *item3 = new wxStaticText( parent, ID_TEXT, "Username:", wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item3, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item4 = new wxTextCtrl( parent, ID_NET_EDIT_YAHOO_USERNAME, "", wxDefaultPosition, wxSize(80,-1), 0 );
    item2->Add( item4, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item5 = new wxStaticText( parent, ID_TEXT, "Password:", wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item5, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item6 = new wxTextCtrl( parent, ID_NET_EDIT_YAHOO_PASSWORD, "", wxDefaultPosition, wxSize(80,-1), wxTE_PASSWORD );
    item2->Add( item6, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item0->Add( item2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item7 = new wxStaticText( parent, ID_TEXT, "Server properties:", wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item8 = new wxFlexGridSizer( 4, 0, 0 );
    item8->AddGrowableCol( 1 );

    wxStaticText *item9 = new wxStaticText( parent, ID_TEXT, "Data host:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item9, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item10 = new wxTextCtrl( parent, ID_NET_EDIT_YAHOO_DATAHOST, "", wxDefaultPosition, wxSize(120,-1), 0 );
    item8->Add( item10, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item11 = new wxStaticText( parent, ID_TEXT, "Port:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item11, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item12 = new wxTextCtrl( parent, ID_NET_EDIT_YAHOO_DATAPORT, "", wxDefaultPosition, wxSize(50,-1), 0 );
    item8->Add( item12, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxStaticText *item13 = new wxStaticText( parent, ID_TEXT, "Auth Host:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item13, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item14 = new wxTextCtrl( parent, ID_NET_EDIT_YAHOO_AUTHHOST, "", wxDefaultPosition, wxSize(120,-1), 0 );
    item8->Add( item14, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item15 = new wxStaticText( parent, ID_TEXT, "Port:", wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item15, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item16 = new wxTextCtrl( parent, ID_NET_EDIT_YAHOO_AUTHPORT, "", wxDefaultPosition, wxSize(50,-1), 0 );
    item8->Add( item16, 0, wxALIGN_CENTRE|wxALL, 5 );

    item0->Add( item8, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxFlexGridSizer *item17 = new wxFlexGridSizer( 1, 0, 0 );

    wxCheckBox *item18 = new wxCheckBox( parent, ID_NET_EDIT_YAHOO_AUTO_LOGIN, "Auto-Login", wxDefaultPosition, wxDefaultSize, 0 );
    item17->Add( item18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item0->Add( item17, 0, wxALIGN_CENTRE|wxALL, 5 );

    if (set_sizer)
    {
        parent->SetAutoLayout( TRUE );
        parent->SetSizer( item0 );
        if (call_fit)
        {
            item0->Fit( parent );
            item0->SetSizeHints( parent );
        }
    }
    
    return item0;
}