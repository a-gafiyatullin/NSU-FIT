#include "MessageUtils.h"
#include "../dht/Node.h"

MessageUtils* MessageUtils::_instance = nullptr;
std::string   MessageUtils::_send_commands[] = {"SEND_PING", "SEND_STOR", "SEND_FVAL", "SEND_FNOD" };
std::string   MessageUtils::_ans_commands[] = {"ANSW_PING", "ANSW_FVAL", "ANSW_FNOD" };
ParsedMessage ParsedMessage::NO_PARSED_MESSAGE = {ANS_UNKNOWN, SEND_UNKNOWN, {}, nullptr, 0, {}, {}, 0};

MessageUtils *MessageUtils::instance() {
  return (_instance == nullptr ? (_instance = new MessageUtils()) : _instance);
}

void MessageUtils::release() {
  delete _instance;
  _instance = nullptr;
}

ParsedMessage MessageUtils::parse_recved_send_msg(const SEND_MSG_TYPES &type, void *buf, const size_t &size) {
  ParsedMessage msg;

  msg._ans_type = ANS_UNKNOWN;
  msg._send_type = type;
  msg._time = time(nullptr);

  size_t pos = COMMAND_SIZE;
  for(int i = 0; i < SHA_DIGEST_LENGTH; i++) {
    msg._id.push_back(((unsigned char*)buf + pos)[i]);
  }
  pos += sizeof (unsigned char) * SHA_DIGEST_LENGTH;

  if(type != SEND_PING) {
    for(int i = 0; i < SHA_DIGEST_LENGTH; i++) {
      msg._hash.push_back(((unsigned char*)buf + pos)[i]);
    }
  }
  pos += sizeof (unsigned char) * SHA_DIGEST_LENGTH;

  if(type == SEND_STORE) {
    msg._buf = (char*)buf + pos;
    msg._buf_size = size - pos;
  }

  return msg;
}

ParsedMessage MessageUtils::parse_recved_ans_msg(const ANS_MSG_TYPES &type, void *buf, const size_t &size) {
  ParsedMessage msg;

  msg._buf = nullptr;
  msg._buf_size = 0;
  msg._ans_type = type;
  msg._send_type = SEND_UNKNOWN;
  msg._time = time(nullptr);

  size_t pos = COMMAND_SIZE;
  for(int i = 0; i < SHA_DIGEST_LENGTH; i++) {
    msg._id.push_back(((unsigned char*)buf + pos)[i]);
  }
  pos += sizeof (unsigned char) * SHA_DIGEST_LENGTH;

  if(msg._ans_type == ANS_PING) {
    return msg;
  }

  int number = *((int*)((char*)buf + pos));
  pos += sizeof (int);

  if(number < 0 && msg._ans_type == ANS_FIND_VALUE) {
    msg._buf = (char*)buf + pos;
    msg._buf_size = size - pos;

    return msg;
  }

  for(int i = 0; i < number; i++) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = *(uint32_t*)((char*)buf + pos);
    addr.sin_port = *(uint32_t*)((char*)buf + pos + sizeof (uint32_t));

    msg._nodes.push_back(std::make_shared<Node>(time(nullptr), (sockaddr *)&addr, sizeof (addr), msg._id));

    pos += sizeof (uint32_t) + sizeof (in_port_t);
  }

  return msg;
}

bool ParsedMessage::is_no_parsed_msg(const ParsedMessage &msg) {
  return msg._ans_type == NO_PARSED_MESSAGE._ans_type && msg._send_type == NO_PARSED_MESSAGE._send_type;
}

void* MessageUtils::gen_send_msg_to_node(size_t &size
                                         , const value_hash_or_node_id &data
                                         , const SEND_MSG_TYPES &type, const node_id &from) {

  assert(type != SEND_STORE);

  char *buf = new char[(type == SEND_PING
                        ? (size = (COMMAND_SIZE + SHA_DIGEST_LENGTH))
                        : (size = (COMMAND_SIZE + 2 * SHA_DIGEST_LENGTH)))];

  if(buf != nullptr) {
    memcpy(buf, _send_commands[type].data(), COMMAND_SIZE);
    memcpy(buf + COMMAND_SIZE, from.data(), SHA_DIGEST_LENGTH);
  } else {
    size = -1;
    return buf;
  }

  if(type != SEND_PING) {
    memcpy(buf + COMMAND_SIZE + SHA_DIGEST_LENGTH, data.data(), SHA_DIGEST_LENGTH);
  }

  return buf;
}

void *MessageUtils::gen_ans_msg_to_node(size_t &size
                                       , const std::vector<std::shared_ptr<Node>> &data
                                       , const ANS_MSG_TYPES &type, const node_id &from) {
  int bucket_size = data.size();

  char *buf = new char[(type == ANS_PING
                        ? (size = (COMMAND_SIZE + SHA_DIGEST_LENGTH))
                        : (size = (COMMAND_SIZE + SHA_DIGEST_LENGTH
                                   + sizeof (int) + bucket_size * (sizeof (uint32_t) + sizeof (in_port_t)))))];

  if(buf != nullptr) {
    memcpy(buf, _ans_commands[type].data(), COMMAND_SIZE);
    memcpy(buf + COMMAND_SIZE, from.data(), SHA_DIGEST_LENGTH);
  } else {
    size = -1;
    return buf;
  }

  if(type != ANS_PING) {
    size_t pos = COMMAND_SIZE + SHA_DIGEST_LENGTH;
    memcpy(buf + pos, &bucket_size, sizeof (int));
    pos += sizeof (int);

    for(int i = 0; i < bucket_size; i++) {
       memcpy(buf + pos, data[i]->id().data(), SHA_DIGEST_LENGTH);
       pos += SHA_DIGEST_LENGTH;

       auto addr = data[i]->address();
       memcpy(buf + pos, &addr.first, sizeof (uint32_t));
       pos += sizeof (uint32_t);
       memcpy(buf + pos, &addr.second, sizeof (in_port_t));
       pos += sizeof (in_port_t);
    }
  }

  return buf;
}

void *MessageUtils::gen_str_msg_to_node(size_t &size
                                        , const value_hash &key
                                        , const void *value, const size_t &value_size
                                        , const node_id &from) {

  size = COMMAND_SIZE + 2 * SHA_DIGEST_LENGTH + value_size;
  char *buf = new char[size];

  if(buf != nullptr) {
    memcpy(buf, _send_commands[SEND_STORE].data(), COMMAND_SIZE);
    memcpy(buf + COMMAND_SIZE, from.data(), SHA_DIGEST_LENGTH);
    memcpy(buf + COMMAND_SIZE + SHA_DIGEST_LENGTH, key.data(), SHA_DIGEST_LENGTH);
    memcpy(buf + COMMAND_SIZE + 2 * SHA_DIGEST_LENGTH, value, value_size);
  } else {
    size = -1;
  }

  return buf;
}

void *MessageUtils::gen_find_val_msg_to_node(size_t &size
                                             , const void *data, const size_t &data_size
                                             , const node_id &from) {
  size = COMMAND_SIZE + SHA_DIGEST_LENGTH + sizeof (int) + data_size;
  char *buf = new char[size];

  if(buf != nullptr) {
    memcpy(buf, _ans_commands[ANS_FIND_VALUE].data(), COMMAND_SIZE);
    memcpy(buf + COMMAND_SIZE, from.data(), SHA_DIGEST_LENGTH);
    int found = -1;
    memcpy(buf + COMMAND_SIZE + SHA_DIGEST_LENGTH, &found, sizeof (int));
    memcpy(buf + COMMAND_SIZE + SHA_DIGEST_LENGTH + sizeof (int), data, data_size);
  } else {
    size = -1;
  }

  return buf;
}

ParsedMessage MessageUtils::parse_recved_msg(void *buf, const size_t &size) {

  if(size < COMMAND_SIZE + SHA_DIGEST_LENGTH) {
    return ParsedMessage::NO_PARSED_MESSAGE;
  }

  ANS_MSG_TYPES   ans_type  = ANS_UNKNOWN;
  SEND_MSG_TYPES  send_type = SEND_UNKNOWN;

  std::string type((char*)buf, COMMAND_SIZE - 1);
  for(int i = 0; i < ANS_COMMANDS_COUNT; i++) {
    if(type == _ans_commands[i]) {
      ans_type = (ANS_MSG_TYPES)i;
      break;
    }
  }

  if(ans_type != ANS_UNKNOWN) {
    return parse_recved_ans_msg(ans_type, buf, size);
  }

  for(int i = 0; i < SEND_COMMANDS_COUNT; i++) {
    if(type == _send_commands[i]) {
      send_type = (SEND_MSG_TYPES)i;
      break;
    }
  }

  if(send_type != SEND_UNKNOWN) {
    return parse_recved_send_msg(send_type, buf, size);
  }

  return ParsedMessage::NO_PARSED_MESSAGE;
}
