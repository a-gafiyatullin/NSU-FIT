# SIMPLE SOCKS5 PROXY SERVER
## About:
Simple Socks5 Proxy Server.

## Features:
<ul>
<li>All blocking system calls(send, recv, connect and accept) are multiplexed in select(2) call.</li>
<li>Resolve DNS queries using multiplexed c-ares library operations and callback mechanism.</li>
<li>Only "CONNECT" (0x1) operation is available.</li>
</ul>

## Dependencies:
<ul>
<li><strong>c-ares:</strong><br>
<code>sudo aptitude install libc-ares-dev</code>
</li>

<li><strong>C++ 20:</strong><br>
<code>sudo aptitude install g++</code>
</li>
</ul>

## Execution:
To start proxy server using specified port PORT type command:<br>
<code>./proxy -p PORT</code><br>
To see more debug info build project in debug mode using cmake or just compile with -DEBUG compiling option.
