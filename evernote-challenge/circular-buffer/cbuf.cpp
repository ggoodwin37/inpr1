// cbuf.cpp: implement a circular buffer as spec'd by the evernote interview challenge #1 (see prompt.txt)
//  ggoodwin 1/5/2013
#include <list>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <cctype>  // tolower

////////////////////////////////////////////////////////////////////////////////////////////////////
// CBufCommand and related

class CBufCommand
{
public:
  virtual void describe() = 0;
};

///////////////

class CBufSizeCommand : public CBufCommand
{
  const unsigned m_size;
public:
  CBufSizeCommand( unsigned _size ) : m_size( _size ) {}
  void describe();
};

void CBufSizeCommand::describe()
{
  std::cout << "Size: " << m_size << std::endl;
}

///////////////

class CBufAppendCommand : public CBufCommand
{
  const std::list<std::string> m_appendList;
public:
  CBufAppendCommand( std::list<std::string> _appendList ) : m_appendList( _appendList ) {}
  void describe();
};

void describe_one_value( const std::string& _str )
{
  std::cout << _str << " ";
}

void CBufAppendCommand::describe()
{
  std::cout << "Append values: ";
  std::for_each( m_appendList.begin(), m_appendList.end(), &describe_one_value );
  std::cout << std::endl;
}

///////////////

class CBufRemoveCommand : public CBufCommand
{
  const unsigned m_removeNum;
public:
  CBufRemoveCommand( unsigned _removeNum ) : m_removeNum( _removeNum ) {}
  void describe();
};

void CBufRemoveCommand::describe()
{
  std::cout << "Remove " << m_removeNum << " value(s)" << std::endl;
}

///////////////

class CBufListCommand : public CBufCommand
{
public:
  CBufListCommand() {}
  void describe();
};

void CBufListCommand::describe()
{
  std::cout << "List contents" << std::endl;
}

///////////////

class CBufQuitCommand : public CBufCommand
{
public:
  CBufQuitCommand() {}
  void describe();
};

void CBufQuitCommand::describe()
{
  std::cout << "Quit" << std::endl;
}

///////////////

class CBufCommandError : public CBufCommand
{
public:
  CBufCommandError() {}
  void describe();
};

void CBufCommandError::describe()
{
  std::cout << "Error" << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// InputStateMachine and related

class InputStateMachine
{
  bool                  m_didReadSize;
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

  bool fDone( false );
  while( !fDone )
  {
    std::auto_ptr<CBufCommand> pCmd = ism.getNextCommand( fDone );
    pCmd->describe();
  }

  // TODO: add parsing logic to ISM, then loop through commands and describe them for now.
  //       once that's done, add the circular buffer.

  return 0;
}
