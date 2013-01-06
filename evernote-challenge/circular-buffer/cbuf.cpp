// cbuf.cpp: implement a circular buffer as spec'd by the evernote interview challenge #1 (see prompt.txt)
//  ggoodwin 1/5/2013
#include <list>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <cctype>  // tolower
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
// CircularBuffer

class CircularBuffer
{
  unsigned m_maxSize;
  unsigned m_insertPoint;
  unsigned m_itemCount;

  std::vector<std::string> m_buf;

  int normalizeIndex( const int index ) const;

public:
  CircularBuffer() : m_maxSize( 0 ),
                     m_insertPoint( 0 ),
                     m_itemCount( 0 ),
                     m_buf( 0 )
  {
  }

  void setSize( const unsigned size );
  void append( const std::list<std::string> list );
  void remove( const unsigned count );
  void showList( void );
};

int CircularBuffer::normalizeIndex( const int index ) const 
{
  int result = index;
  while( result < 0 )
    result += m_maxSize;
  result %= m_maxSize;
  return result;
}

void CircularBuffer::setSize( const unsigned size )
{
  m_maxSize = size;
  m_buf.resize( size );
  
  // buffer state becomes undefined when size changes,
  // this assumes that size will be set before anything
  // is added to buffer (a valid assumption according to spec,
  // since size always comes first).
}

void CircularBuffer::append( const std::list<std::string> list )
{
  // we want an ordered append so avoid for_each
  for( std::list<std::string>::const_iterator i = list.begin(); i != list.end(); ++i )
  {
    m_buf[ m_insertPoint ] = *i;
    m_insertPoint = normalizeIndex( m_insertPoint + 1 );
    m_itemCount = std::min( m_itemCount + 1, m_maxSize );
  }
}

void CircularBuffer::remove( const unsigned count )
{
  m_itemCount -= count;
}

void CircularBuffer::showList( void )
{
  for( int i = 0; i < m_itemCount; ++i )
  {
    int thisIndex = normalizeIndex( m_insertPoint - m_itemCount + i );
    std::cout << m_buf[ thisIndex ] << std::endl;
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// CBufCommand and related

class CBufCommand
{
public:
  virtual void perform( CircularBuffer& cb ) const = 0;
};

///////////////

class CBufSizeCommand : public CBufCommand
{
  const unsigned m_size;
public:
  CBufSizeCommand( unsigned _size ) : m_size( _size ) {}
  void perform( CircularBuffer& cb ) const;
};

void CBufSizeCommand::perform( CircularBuffer& cb ) const
{
  cb.setSize( m_size );
}

///////////////

class CBufAppendCommand : public CBufCommand
{
  const std::list<std::string> m_appendList;
public:
  CBufAppendCommand( std::list<std::string> _appendList ) : m_appendList( _appendList ) {}
  void perform( CircularBuffer& cb ) const;
};

void CBufAppendCommand::perform( CircularBuffer& cb ) const
{
  cb.append( m_appendList );
}

///////////////

class CBufRemoveCommand : public CBufCommand
{
  const unsigned m_removeNum;
public:
  CBufRemoveCommand( unsigned _removeNum ) : m_removeNum( _removeNum ) {}
  void perform( CircularBuffer& cb ) const;
};

void CBufRemoveCommand::perform( CircularBuffer& cb ) const
{
  cb.remove( m_removeNum );
}

///////////////

class CBufListCommand : public CBufCommand
{
public:
  CBufListCommand() {}
  void perform( CircularBuffer& cb ) const;
};

void CBufListCommand::perform( CircularBuffer& cb ) const
{
  cb.showList();
}

///////////////

class CBufQuitCommand : public CBufCommand
{
public:
  CBufQuitCommand() {}
  void perform( CircularBuffer& cb ) const;
};

void CBufQuitCommand::perform( CircularBuffer& cb ) const
{
  // do nothing
}

///////////////

class CBufCommandError : public CBufCommand
{
public:
  CBufCommandError() {}
  void perform( CircularBuffer& cb ) const;
};

void CBufCommandError::perform( CircularBuffer& cb ) const
{
  // do nothing
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// InputParser

class InputParser
{
  bool            m_didReadSize;
  std::istream&   m_istream;

  static unsigned readCountArg( const std::string& _str );

  const std::list<std::string> getAppendList( const unsigned numLines );

public:
  InputParser( std::istream& _istream ) :
    m_istream( _istream ),
    m_didReadSize( false )
  { }

  std::auto_ptr<const CBufCommand> getNextCommand( /*out*/ bool& fDone );
};

/*static*/ unsigned InputParser::readCountArg( const std::string& _str )
{
  std::string trimmed = _str.substr( 2 );
  return atoi( trimmed.c_str() );
}

const std::list<std::string> InputParser::getAppendList( const unsigned numLines )
{
  std::list<std::string> appendList;
  for( int i = 0; i < numLines; ++i )
  {
    std::string thisAppend;
    std::getline( m_istream, thisAppend );
    appendList.push_back( thisAppend );
  }
  return appendList;
}

std::auto_ptr<const CBufCommand> InputParser::getNextCommand( /*out*/ bool& fDone )
{
  fDone = false;

  std::string thisLine;
  std::getline( m_istream, thisLine );
  if( !m_didReadSize )
  {
    const unsigned size = atoi( thisLine.c_str() );
    m_didReadSize = true;
    return std::auto_ptr<const CBufCommand>( new CBufSizeCommand( size ) );
  }

  const char firstChar = tolower( thisLine[ 0 ] );
  switch( firstChar )
  {
  case 'a':
    return std::auto_ptr<const CBufCommand>( new CBufAppendCommand( getAppendList( InputParser::readCountArg( thisLine ) ) ) );
  case 'r':
    return std::auto_ptr<const CBufCommand>( new CBufRemoveCommand( InputParser::readCountArg( thisLine ) ) );
  case 'l':
    return std::auto_ptr<const CBufCommand>( new CBufListCommand() );
  case 'q':
    fDone = true;
    return std::auto_ptr<const CBufCommand>( new CBufQuitCommand() );
  default:
    fDone = true;
    return std::auto_ptr<const CBufCommand>( new CBufCommandError() );
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// main

int main( int argc, char **argv )
{
  InputParser ip( std::cin );
  CircularBuffer cb;

  bool fDone( false );
  while( !fDone )
  {
    std::auto_ptr<const CBufCommand> pCmd = ip.getNextCommand( fDone );
    pCmd->perform( cb );
  }

  return 0;
}
