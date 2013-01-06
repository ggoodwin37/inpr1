// frequent-terms.cpp: implement the "frequent terms" algorithm as spec'd by the evernote interview challenge (see prompt.txt)
//  ggoodwin 1/6/2013
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include <cstdlib>   // atoi
//#include <list>
//#include <algorithm>
//#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
// TermCountMap
class TermCountMap
{
  std::map<std::string, unsigned> m_map;
  unsigned m_entriesRemaining;

public:
  TermCountMap() : m_map(), m_entriesRemaining( 0 )
  {
  }

  void handle( const std::string newValue );
  void setEntriesRemaining( const unsigned remaining );
  void showKResults( const unsigned k );
};

void TermCountMap::handle( const std::string newValue )
{
  // TODO
}

void TermCountMap::setEntriesRemaining( const unsigned remaining )
{
  m_entriesRemaining = remaining;
}

void TermCountMap::showKResults( const unsigned k )
{
  // TODO
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Commands

class Command
{
public:
  virtual void perform( TermCountMap& tcm ) const = 0;
};

///////////////

class SizeCommand : public Command
{
  const unsigned m_size;
public:
  SizeCommand( const unsigned _size ) : m_size( _size ) {}
  void perform( TermCountMap& tcm ) const;
};

void SizeCommand::perform( TermCountMap& tcm ) const
{
  tcm.setEntriesRemaining( m_size );
}

///////////////

class NewEntryCommand : public Command
{
  const std::string m_entry;
public:
  NewEntryCommand( const std::string entry ) : m_entry( entry ) {}
  void perform( TermCountMap& tcm ) const;
};

void NewEntryCommand::perform( TermCountMap& tcm ) const
{
  tcm.handle( m_entry );
}

///////////////

class ShowNMostCommonCommand : public Command
{
  const unsigned m_numToShow;
public:
  ShowNMostCommonCommand( const unsigned numToShow ) : m_numToShow( numToShow ) {}
  void perform( TermCountMap& tcm ) const;
};

void ShowNMostCommonCommand::perform( TermCountMap& tcm ) const
{
  tcm.showKResults( m_numToShow );
}

///////////////

class QuitCommand : public Command
{
public:
  QuitCommand() {}
  void perform( TermCountMap& tcm ) const;
};

void QuitCommand::perform( TermCountMap& tcm ) const
{
  // do nothing
}

///////////////

class CommandError : public Command
{
public:
  CommandError() {}
  void perform( TermCountMap& tcm ) const;
};

void CommandError::perform( TermCountMap& tcm ) const
{
  std::cout << "error: unexpected state." << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// InputParser

class InputParser
{
  bool            m_didReadSize;
  std::istream&   m_istream;

  unsigned m_wordsRemaining;

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
    m_wordsRemaining = atoi( thisLine.c_str() );
    m_didReadSize = true;
    return std::auto_ptr<const Command>( new SizeCommand( m_wordsRemaining ) );
  }

  if( m_wordsRemaining > 0 )
  {
    m_wordsRemaining--;
    return std::auto_ptr<const Command>( new NewEntryCommand( thisLine ) );
  }

  const unsigned displayN = atoi( thisLine.c_str() );
  if( displayN > 0 )
  {
    return std::auto_ptr<const Command>( new ShowNMostCommonCommand( displayN ) );
  }

  // unexpected input state
  fDone = true;
  return std::auto_ptr<const Command>( new CommandError() );
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// main

int main( int argc, char **argv )
{
  InputParser ip( std::cin );
  TermCountMap tcm;

  bool fDone( false );
  while( !fDone )
  {
    std::auto_ptr<const Command> pCmd = ip.getNextCommand( fDone );
    pCmd->perform( tcm );
  }

  return 0;
}
