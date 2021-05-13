# Distributed Files Storage

### This version of the system doesn't work!

## <strong>Build</strong>:
From the root of the project:<br>
<code>cmake .<br>make</code><br>
File <code>laboratory-work-5</code> will be generated.

## <strong>Run</strong>:
<code>./laboratory-work-5 [dht-port] [io-port] [bootstrap-node-info-file]</code><br>
<code>dht-port</code> - port for the interaction between nodes<br>
<code>io-port</code>  - port for the download requests from this node<br>
<code>bootstrap-node-info-file</code>  - file with the information about another node that is in the net:<br>
File format by lines:<br>
1st - hash of the known node (20 values between 0 and 255)<br>
2nd - IPv4 address of the known node<br>
3rd - dht port of the known node<br>
<strong>Example:</strong><br>
<code>57 129 115 203 226 165 20 59 251 70 58 235 246 157 68 112 223 227 219 160<br>
0.0.0.0<br>
8088</code>

## <strong>Usage</strong>:
Commands:<br>
<code>a</code> - add a new file to the system. The name and the path of the file are required<br>
<code>f</code> - find the file in the system. The name and the destination path of the file are required<br>
<code>s</code> - stop the system.

## <strong>Protocol</strong>:
The protocol was described in the paper <code>Kademlia: A Peer-to-peer Information System
Based on the XOR Metric</code> by Petar Maymounkov and David Mazières.<br>
You can find this paper in <code>docs</code>.
