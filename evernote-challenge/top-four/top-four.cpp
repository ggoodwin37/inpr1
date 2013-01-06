// top-four.cpp: implement the "top four" algorithm as spec'd by the evernote interview challenge (see prompt.txt)
//  ggoodwin 1/6/2013
#include <list>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <climits> // LONG_MIN
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
// TopNBuffer
class TopNBuffer
{
  std::vector<long int> m_buf;  // descending, zero'th element is largest
  const unsigned m_bufSize;
  unsigned m_entriesRemaining;

  static void insertValue( const long int newValue, std::vector<long int>::iterator it, std::vector<long int>::iterator end );

public:
  TopNBuffer( const unsigned bufSize ) : m_bufSize( bufSize ), m_buf( bufSize, LONG_MIN ), m_entriesRemaining( 0 )
  {
  }

  void handle( const long int newValue );
  void setEntriesRemaining( const unsigned remaining );
  void showResults( void );
  
};

/*static*/ void TopNBuffer::insertValue( const long int newValue, std::vector<long int>::iterator it, std::vector<long int>::iterator end )
{
  const long int oldValue = *it;
  *it = newValue;
  if( ++it != end )
  {
    insertValue( oldValue, it, end );
  }
}

void TopNBuffer::handle( const long int newValue )
{
  if( m_entriesRemaining == 0 )
  {
    return;
  }
  m_entriesRemaining--;

  for( std::vector<long int>::iterator it = m_buf.begin(); it != m_buf.end(); ++it )
  {
    if( newValue >= *it )
    {
      insertValue( newValue, it, m_buf.end() );
      return;
    }
  }
}

void TopNBuffer::setEntriesRemaining( const unsigned remaining )
{
  m_entriesRemaining = remaining;
}

void TopNBuffer::showResults( void )
{
  for( std::vector<long int>::const_iterator it = m_buf.begin(); it != m_buf.end(); ++it )
  {
    std::cout << *it << std::endl;
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Commands

class Command
{
public:
  virtual void perform( TopNBuffer& buf ) const = 0;
};

///////////////

class SizeCommand : public Command
{
  const unsigned m_size;
public:
  SizeCommand( const unsigned _size ) : m_size( _size ) {}
  void perform( TopNBuffer& buf ) const;
};

void SizeCommand::perform( TopNBuffer& cb ) const
{
  cb.setEntriesRemaining( m_size );
}

///////////////

class NewEntryCommand : public Command
{
  const long int m_entryValue;
public:
  NewEntryCommand( const long int entryValue ) : m_entryValue( entryValue ) {}
  void perform( TopNBuffer& buf ) const;
};

void NewEntryCommand::perform( TopNBuffer& buf ) const
{
  buf.handle( m_entryValue );
}

///////////////

class QuitCommand : public Command
{
public:
  QuitCommand() {}
  void perform( TopNBuffer& buf ) const;
};

void QuitCommand::perform( TopNBuffer& buf ) const
{
  // do nothing
}

///////////////

class CommandError : public Command
{
public:
  CommandError() {}
  void perform( TopNBuffer& buf ) const;
};

void CommandError::perform( TopNBuffer& buf ) const
{
  // do nothing
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// InputParser

class InputParser
{
  bool            m_didReadSize;
  std::istream&   m_istream;

public:
  InputParser( std::istream& _istream ) :
    m_istream( _istream ),
    m_didReadSize( false )
  { }

  std::auto_ptr<const Command> getNextCommand( /*out*/ bool& fDone );
};

std::auto_ptr<const Command> InputParser::getNextCommand( /*out*/ bool& fDone )
{
  fDone = false;

  std::string thisLine;
  if( !std::getline( m_istream, thisLine ) )
  {
    fDone = true;
    return std::auto_ptr<const Command>( new QuitCommand() );
  }

  if( !m_didReadSize )
  {
    const unsigned size = atoi( thisLine.c_str() );
    m_didReadSize = true;
    return std::auto_ptr<const Command>( new SizeCommand( size ) );
  }

  const long int newEntry = atol( thisLine.c_str() );
  return std::auto_ptr<const Command>( new NewEntryCommand( newEntry ) );
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// main

int main( int argc, char **argv )
{
  InputParser ip( std::cin );
  TopNBuffer buf( 4 );

  bool fDone( false );
  while( !fDone )
  {
    std::auto_ptr<const Command> pCmd = ip.getNextCommand( fDone );
    pCmd->perform( buf );
  }
  buf.showResults();

  return 0;
}
