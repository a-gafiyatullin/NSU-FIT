#pragma once

#include <arpa/inet.h>
#include "MessageUtils.h"

class Node {
  protected:
    ulli                    _time;

    sockaddr_in             _addr;
    socklen_t               _socklen;

    node_id                 _id;

    bool send_helper(const value_hash_or_node_id &data, int socket, const SEND_MSG_TYPES &type, const node_id &from);
  public:
    Node(const ulli &time, const sockaddr *addr, const socklen_t &socklen, const node_id& id);

    value_hash distance(const value_hash& hash);

    value_hash distance(const Node& node);

    ulli time() const { return _time; }

    void update_time(const ulli &time);

    bool is_your_id(const node_id &id);

    const node_id &id() const { return _id; }

    std::pair<uint32_t, in_port_t> address() const { return std::make_pair(_addr.sin_addr.s_addr, _addr.sin_port); }

    // ---------------- Kademlia protocol ----------------

    bool find_node(const node_id& id, int socket, const node_id &from);

    bool find_value(const value_hash& hash, int socket, const node_id &from);

    bool store(const value_hash& hash, const void *data, const size_t &data_size, int socket, const node_id &from);

    bool ping(int socket, const node_id &from);
};
