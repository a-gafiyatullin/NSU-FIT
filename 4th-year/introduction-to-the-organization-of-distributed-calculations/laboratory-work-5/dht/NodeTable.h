#pragma once

#include "Node.h"
#include <time.h>
#include <stdexcept>
#include <unistd.h>
#include <algorithm>
#include <queue>
#include <mutex>
#include <thread>
#include "../storage/Storage.h"

using k_bucket              = std::list<std::shared_ptr<Node>>;

class NodeTable {
  protected:
    struct ReplaceTask {
        std::shared_ptr<Node> _node_for_replace;
        ulli                  _time;
    };

    std::vector<ReplaceTask>  _replace_tasks;

    struct Message {
        ParsedMessage _msg;

        void*       _buf;
        sockaddr_in _addr;
        socklen_t   _len;

        static Message NO_MESSAGE;

        static bool is_no_message(const Message &msg);
    };

    std::queue<Message>     _msg_queue;

    static const int BUFF_SIZE              = 4096;
    static const int SHA_BITS               = 160;
    static const int K_BUCKET_SIZE          = 20;
    static const int DELAY_FOR_REPLACEMENT  = 10;
    static const int LOOKUP_TIME            = 2;
    static const int ALPHA                  = 5;

    int                   _socket;

    std::shared_ptr<Node> _self;

    std::vector<k_bucket> _table;

    static NodeTable*     _instance;

    int k_bucket_order(const node_id &id) const;

    void update_k_bucket(const node_id &id, const sockaddr *addr, const socklen_t &len);

    void update_k_bucket(const std::vector<std::shared_ptr<Node>> &nodes);

    void update_nodes();

    bool conditional_update(std::list<std::shared_ptr<Node>> &nearest_nodes
                            , const std::vector<std::shared_ptr<Node>> &possible_nodes
                            , const value_hash_or_node_id &pivot);

    Message filtered_recv(const std::vector<ANS_MSG_TYPES> &desired_ans, const std::vector<SEND_MSG_TYPES> &desired_sends);

    std::list<std::shared_ptr<Node>> node_lookup(const node_id &id);

    NodeTable(const int &port
              , const node_id &bootstrap_node
              , const std::string &bootstrap_addr, const in_port_t &bootstrap_port);

    std::mutex    _service_loop_lock;
    volatile bool _stop;
    std::thread   _service_thread;

    void service_loop();
    void service_ans_msg(const Message &msg);
    void service_send_msg(const Message &msg);
  public:
    // ---------------- Kademlia protocol ----------------
    static NodeTable *init(const int &port
                           , const node_id &bootstrap_node
                           , const std::string &bootstrap_addr, const in_port_t &bootstrap_port);
    static NodeTable *instance();

    void store(const value_hash &hash, const void *buf, const size_t &size);

    bool value_lookup(const value_hash &hash);

    static void release();
};
