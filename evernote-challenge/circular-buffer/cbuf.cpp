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

  int normalizeIndex( int index );

public:
  CircularBuffer() : m_maxSize( 0 ),
                     m_insertPoint( 0 ),
                     m_itemCount( 0 ),
                     m_buf( 0 )
  {
  }

  void setSize( unsigned size );
  void append( std::list<std::string> list );
  void remove( unsigned count );
  void showList( void );
};

int CircularBuffer::normalizeIndex( int index )
{
  int result = index;
  while( result < 0 )
    result += m_maxSize;
  result %= m_maxSize;
  return result;
}

void CircularBuffer::setSize( unsigned size )
{
  m_maxSize = size;
  m_buf.resize( size );
  
  // buffer state becomes undefined when size changes,
  // this assumes that size will be set before anything
  // is added to buffer (a valid assumption according to spec,
  // since size always comes first).
}

void CircularBuffer::append( std::list<std::string> list )
{
  // we want an ordered append so avoid for_each
  for( std::list<std::string>::iterator i = list.begin(); i != list.end(); ++i )
  {
    m_buf[ m_insertPoint ] = *i;
    m_insertPoint = normalizeIndex( m_insertPoint + 1 );
    m_itemCount = std::min( m_itemCount + 1, m_maxSize );
  }
}

void CircularBuffer::remove( unsigned count )
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
  virtual void perform( CircularBuffer& cb ) = 0;
};

///////////////

class CBufSizeCommand : public CBufCommand
{
  const unsigned m_size;
public:
  CBufSizeCommand( unsigned _size ) : m_size( _size ) {}
  void perform( CircularBuffer& cb );
};

void CBufSizeCommand::perform( CircularBuffer& cb )
{
  cb.setSize( m_size );
}

///////////////

class CBufAppendCommand : public CBufCommand
{
  const std::list<std::string> m_appendList;
public:
  CBufAppendCommand( std::list<std::string> _appendList ) : m_appendList( _appendList ) {}
  void perform( CircularBuffer& cb );
};

void CBufAppendCommand::perform( CircularBuffer& cb )
{
  cb.append( m_appendList );
}

///////////////

class CBufRemoveCommand : public CBufCommand
{
  const unsigned m_removeNum;
public:
  CBufRemoveCommand( unsigned _removeNum ) : m_removeNum( _removeNum ) {}
  void perform( CircularBuffer& cb );
};

void CBufRemoveCommand::perform( CircularBuffer& cb )
{
  cb.remove( m_removeNum );
}

///////////////

class CBufListCommand : public CBufCommand
{
public:
  CBufListCommand() {}
  void perform( CircularBuffer& cb );
};

void CBufListCommand::perform( CircularBuffer& cb )
{
  cb.showList();
}

///////////////

class CBufQuitCommand : public CBufCommand
{
public:
  CBufQuitCommand() {}
  void perform( CircularBuffer& cb );
};

void CBufQuitCommand::perform( CircularBuffer& cb )
{
  // do nothing
}

///////////////

class CBufCommandError : public CBufCommand
{
public:
  CBufCommandError() {}
  void perform( CircularBuffer& cb );
};

void CBufCommandError::perform( CircularBuffer& cb )
{
  // do nothing
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// InputStateMachine and related

class InputStateMachine
{
  bool            m_didReadSize;
  std::istream&   m_istream;

  static unsigned readCountArg( std::string& _str );

  std::list<std::string> getAppendList( unsigned numLines );

public:
  InputStateMachine( std::istream& _istream ) :
    m_istream( _istream ),
    m_didReadSize( false )
  { }

  std::auto_ptr<CBufCommand> getNextCommand( /*out*/ bool& fDone );

};

/*static*/ unsigned InputStateMachine::readCountArg( std::string& _str )
{
  std::string trimmed = _str.substr( 2 );
  return atoi( trimmed.c_str() );
}

std::list<std::string> InputStateMachine::getAppendList( unsigned numLines )
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

std::auto_ptr<CBufCommand> InputStateMachine::getNextCommand( /*out*/ bool& fDone )
{
  fDone = false;

  std::string thisLine;
  std::getline( m_istream, thisLine );
  if( !m_didReadSize )
  {
    unsigned size = atoi( thisLine.c_str() );
    m_didReadSize = true;
    return std::auto_ptr<CBufCommand>( new CBufSizeCommand( size ) );
  }

  char firstChar = tolower( thisLine[ 0 ] );
  switch( firstChar )
  {
  case 'a':
    return std::auto_ptr<CBufCommand>( new CBufAppendCommand( getAppendList( InputStateMachine::readCountArg( thisLine ) ) ) );
  case 'r':
    return std::auto_ptr<CBufCommand>( new CBufRemoveCommand( InputStateMachine::readCountArg( thisLine ) ) );
  case 'l':
    return std::auto_ptr<CBufCommand>( new CBufListCommand() );
  case 'q':
    fDone = true;
    return std::auto_ptr<CBufCommand>( new CBufQuitCommand() );
  default:
    fDone = true;
    return std::auto_ptr<CBufCommand>( new CBufCommandError() );
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// main

int main( int argc, char **argv )
{
  InputStateMachine ism( std::cin );
  CircularBuffer cb;

  bool fDone( false );
  while( !fDone )
  {
    std::auto_ptr<CBufCommand> pCmd = ism.getNextCommand( fDone );
    pCmd->perform( cb );
  }

  return 0;
}
