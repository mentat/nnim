

class TocProtocol: public Protocol
{



private:

  // buffered packet data for partial packet sends
  string m_buffer;
  int m_origLength;
  int m_origType;

  // FLAP sequence

  int m_sequence;

}