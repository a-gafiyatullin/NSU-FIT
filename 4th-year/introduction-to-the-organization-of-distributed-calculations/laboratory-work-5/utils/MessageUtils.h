#pragma once

#include <vector>
#include <memory>
#include <list>
#include <cstring>
#include <string>
#include <cassert>
#include "HashUtils.h"

class Node;

using ulli                  = unsigned long long int;

enum SEND_MSG_TYPES {
  SEND_PING,
  SEND_STORE,
  SEND_FIND_VALUE,
  SEND_FIND_NODE,

  SEND_COMMANDS_COUNT,
  SEND_UNKNOWN
};

enum ANS_MSG_TYPES {
  ANS_PING,
  ANS_FIND_VALUE,
  ANS_FIND_NODE,

  ANS_COMMANDS_COUNT,
  ANS_UNKNOWN
};

struct ParsedMessage {
    ANS_MSG_TYPES                       _ans_type;
    SEND_MSG_TYPES                      _send_type;

    std::vector<std::shared_ptr<Node>>  _nodes;
    void*                               _buf;
    size_t                              _buf_size;

    node_id                             _id;
    value_hash_or_node_id               _hash;

    ulli                                _time;

    static ParsedMessage NO_PARSED_MESSAGE;

    static bool is_no_parsed_msg(const ParsedMessage &msg);
};

class MessageUtils {
  protected:
    static MessageUtils* _instance;

    MessageUtils() {};

    virtual ~MessageUtils() {}

    static const int COMMAND_SIZE = 10;
    static std::string   _send_commands[SEND_COMMANDS_COUNT];
    static std::string   _ans_commands[ANS_COMMANDS_COUNT];

    virtual ParsedMessage parse_recved_send_msg(const SEND_MSG_TYPES &type, void *buf, const size_t &size);
    virtual ParsedMessage parse_recved_ans_msg(const ANS_MSG_TYPES &type, void *buf, const size_t &size);

  public:    
    virtual void* gen_send_msg_to_node(size_t &size
                                      , const value_hash_or_node_id &data
                                      , const SEND_MSG_TYPES &type, const node_id &from);

    virtual void* gen_ans_msg_to_node(size_t &size
                                      , const std::vector<std::shared_ptr<Node>> &data
                                      , const ANS_MSG_TYPES &type, const node_id &from);

    virtual void* gen_str_msg_to_node(size_t &size
                                      , const value_hash &key
                                      , const void *value, const size_t &value_size
                                      , const node_id &from);

    virtual void* gen_find_val_msg_to_node(size_t &size
                                           , const void *data, const size_t &data_size
                                           , const node_id &from);

    virtual ParsedMessage parse_recved_msg(void *buf, const size_t &size);

    static MessageUtils *instance();
    static void release();
};
