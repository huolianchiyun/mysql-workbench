/* 
 * Copyright (c) 2012, 2015, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#pragma once

#include "mysql-parser-common.h"
#include <stack>

/**
 * C++ interface for the ANTLR based MySQL parser.
 */

class MySQLRecognizer;

/**
 * The tree walker allows to easily navigate the AST generated by the recognizer and get all kind
 * of token information (like position, type, content etc.).
 */
class MYSQL_PARSER_PUBLIC_FUNC MySQLRecognizerTreeWalker
{
  friend class MySQLRecognizer; // For protected c-tor.
public:
  MySQLRecognizer *recognizer() { return _recognizer; };
  
  bool next(size_t count = 1);
  bool next_sibling();
  bool previous();
  bool previous_by_index();
  bool previous_sibling();
  bool up();
  bool advance_to_position(int line, int offset);
  bool advance_to_type(unsigned int type, bool recurse);
  void go_to_subquery_start();
  bool skip_token_sequence(unsigned int start_token, ...);
  bool skip_if(unsigned int token, size_t count = 1);
  unsigned int look_ahead(bool recursive);
  unsigned int parent_type();
  unsigned int previous_type();
  
  void reset();
  void push();
  bool pop();
  void remove_tos();
  
  // Properties of the current token.
  bool is(unsigned int type);
  bool is_nil();
  bool is_subtree();
  bool is_identifier();
  bool is_keyword();
  bool is_relation();
  bool is_first_child();
  bool is_number();
  bool is_operator();

  std::string token_text();
  unsigned int token_type();
  unsigned int token_line();
  unsigned int token_start();
  ANTLR3_MARKER token_index();
  size_t token_offset();
  int token_length();

  std::string text_for_tree();

  MySQLQueryType get_current_query_type();
  MySQLQueryType get_main_query_type();

protected:
  MySQLRecognizerTreeWalker(MySQLRecognizer *recognizer, pANTLR3_BASE_TREE tree);
  
  pANTLR3_BASE_TREE get_next(pANTLR3_BASE_TREE node, bool recurse);
  pANTLR3_BASE_TREE get_previous(pANTLR3_BASE_TREE node, bool recurse);
  pANTLR3_BASE_TREE get_previous_by_index(pANTLR3_BASE_TREE node);
  void print_token(pANTLR3_BASE_TREE tree);
private:
  pANTLR3_BASE_TREE _origin;
  pANTLR3_BASE_TREE _tree;
  std::stack<pANTLR3_BASE_TREE> _token_stack;
  std::vector<pANTLR3_BASE_TREE> _token_list; // A list of all tokens in incoming order (no hierarchy).
  MySQLRecognizer *_recognizer;
};

class MYSQL_PARSER_PUBLIC_FUNC MySQLRecognizer : public MySQLRecognitionBase
{
  friend class MySQLRecognizerTreeWalker;
public:
  MySQLRecognizer(long server_version, const std::string &sql_mode, const std::set<std::string> &charsets);
  virtual ~MySQLRecognizer();
  
  void parse(const char *text, size_t length, bool is_utf8, MySQLParseUnit parse_unit);

  std::string dump_tree();
  std::string dump_tree(pANTLR3_BASE_TREE tree, const std::string &indentation);
  
  MySQLRecognizerTreeWalker tree_walker();
  
  virtual void set_sql_mode(const std::string &new_mode);
  virtual void set_server_version(long new_version);
  virtual const char* text();

  long server_version();

  std::string token_text(pANTLR3_BASE_TREE node);
  MySQLQueryType query_type();
  MySQLQueryType query_type(pANTLR3_BASE_TREE node);

  std::string text_for_tree(pANTLR3_BASE_TREE node);
  MySQLToken token_at_index(ANTLR3_MARKER index);

protected:

private:
  class Private;
  Private *d;
};
