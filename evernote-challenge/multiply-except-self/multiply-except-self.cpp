// multiply-except-self.cpp: implement the "multiply except self" algorithm as spec'd by the evernote interview challenge (see prompt.txt)
// you guys are trying to trick me into an O(n^2) implementation but doing it in O(n) is trivial.
//  ggoodwin 1/6/2013
// #include <list>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
// algorithm, so simple that a class would probably be overkill here.

void multiplyExceptSelf( std::vector<long int>& vec )
{
  // calculate the entire product, then divide by each "self" when printing that line's output.
  long int product = 1;
  for( std::vector<long int>::const_iterator it = vec.begin(); it != vec.end(); ++it )
  {
    product *= *it;
  }
  for( std::vector<long int>::const_iterator it = vec.begin(); it != vec.end(); ++it )
  {
    std::cout << (product / *it) << std::endl;
  }  
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Commands

class Command
{
public:
  virtual void perform( std::vector<long int>& vec ) const = 0;
};

///////////////

class SizeCommand : public Command
{
  const unsigned m_size;
public:
  SizeCommand( const unsigned _size ) : m_size( _size ) {}
  void perform( std::vector<long int>& vec ) const;
};

void SizeCommand::perform( std::vector<long int>& vec ) const
{
  vec.resize( m_size );
}

///////////////

class NewEntryCommand : public Command
{
  const int m_index;
  const long int m_entryValue;

public:
  NewEntryCommand( const int index, const long int entryValue ) : m_index( index ), m_entryValue( entryValue ) {}
  void perform( std::vector<long int>& vec ) const;
};

void NewEntryCommand::perform( std::vector<long int>& vec ) const
{
  vec[ m_index ] = m_entryValue;
}

///////////////

class QuitCommand : public Command
{
public:
  QuitCommand() {}
  void perform( std::vector<long int>& vec ) const;
};

void QuitCommand::perform( std::vector<long int>& vec ) const
{
  // do nothing
}

///////////////

class CommandError : public Command
{
public:
  CommandError() {}
  void perform( std::vector<long int>& vec ) const;
};

void CommandError::perform( std::vector<long int>& vec ) const
{
  // do nothing
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// InputParser

class InputParser
{
  bool            m_didReadSize;
  std::istream&   m_istream;
  unsigned        m_totalElements;
  unsigned        m_nextIndex;

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
    m_totalElements = size;
    m_nextIndex = 0;
    return std::auto_ptr<const Command>( new SizeCommand( size ) );
  }
  
  if( m_nextIndex < m_totalElements )
  {
    long int newValue = atol( thisLine.c_str() );
    return std::auto_ptr<const Command>( new NewEntryCommand( m_nextIndex++, newValue ) );
  }

  fDone = true;
  return std::auto_ptr<const Command>( new CommandError() );  
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// main

int main( int argc, char **argv )
{
  InputParser ip( std::cin );
  std::vector<long int> vec;

  bool fDone( false );
  while( !fDone )
  {
    std::auto_ptr<const Command> pCmd = ip.getNextCommand( fDone );
    pCmd->perform( vec );
  }
  multiplyExceptSelf( vec );

  return 0;
}
