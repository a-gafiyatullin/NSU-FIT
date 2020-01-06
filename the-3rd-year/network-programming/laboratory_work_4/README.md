# Distributed Snake Game
## About:
Classic snake game with network mode support.
## Features:
<ul>
<li>Modern protobuf protocol by Google</li>
<li>Set configurations using simple XML files</li>
<li>Nodes crashing resistant</li>
<li>Open protocol of the nodes interaction</li>
</ul>

## Dependencies:
<ul>
<li><strong>Curses:</strong><br>
<code>sudo aptitude install libncurses5-dev libncursesw5-dev</code>
</li>

<li><strong>Protobuf:</strong><br>
<code>sudo aptitude install protobuf-compiler libprotobuf-dev</code>
</li>

<li><strong>tinyXML:</strong><br>
<code>sudo aptitude install libtinyxml-dev</code>
</li>

<li><strong>C++ 20:</strong><br>
<code>sudo aptitude install g++</code>
</li>
</ul>

## User interface:
<ul>
<li>In the game searching menu you can see up to 10 games that you can join.</li>
<li>To choose one of them just press digit 0-9.</li>
<li>To quite from the game press 'q'.</li>
</ul> 

![alt text](https://github.com/xp10rd/NSU-FIT/blob/master/the-3rd-year/network-programming/laboratory_work_4/samples/game_announce_0.png "Menu of the game searching")
![alt text](https://github.com/xp10rd/NSU-FIT/blob/master/the-3rd-year/network-programming/laboratory_work_4/samples/game_announce_1.png "Menu of the game searching with other implementations")
<ul>
<li>Game window is consist of the two areas:
    <ul>
    <li>game field with snakes on the left side;</li>
    <li>players statistic on the right side.</li>
    </ul>
</li>

<li>For each player in the player statistic area you can see two letters:</li>
    <ul>
    <li><strong>M</strong> - for the MASTER node, <strong>D</strong> - for the DEPUTY node, <strong>N</strong> - for the NORMAL node;</li>
    <li><strong>L</strong> - for your player's statistic, <strong>R</strong> - for other player's statistic.</li>
    </ul>
<li>To control your snake use arrow keys.</li>
<li>To quite from the game press 'q'.</li>
</ul> 

![alt text](https://github.com/xp10rd/NSU-FIT/blob/master/the-3rd-year/network-programming/laboratory_work_4/samples/game_process_0.png "Game process")
![alt text](https://github.com/xp10rd/NSU-FIT/blob/master/the-3rd-year/network-programming/laboratory_work_4/samples/game_process_1.png "Game process with other implementations")
## Config files:
Config file for the master node with all attributes:
```<?xml version="1.0"?>
<config>
    <!-- Settings for Snake -->
    <master>
        <width>80</width>
        <height>25</height>
        <food_static>3</food_static>
        <food_per_player>2.5</food_per_player>
        <state_delay_ms>100</state_delay_ms>
        <dead_food_prob>0.5</dead_food_prob>
        <ping_delay_ms>400</ping_delay_ms>
        <node_timeout_ms>800</node_timeout_ms>
    </master>
    <node>
        <name>Albert</name>
        <only_view>true</only_view>
    </node>
</config>
```
"master" section of this file is required to start your local node as a master.<br><br>
Absence of the "master" section of this file is required to start your local node as a normal.<br>
For example, this config is suitable for the normal node:
```<?xml version="1.0"?>
<config>
    <!-- Settings for Snake -->
    <node>
        <name>Albert</name>
        <only_view>false</only_view>
    </node>
</config>
```
Description of this attributes you can find in <code>src/protobuf/snakes.proto</code> of the project.
## Known issues:
<ul>
    <li>Sometimes game is down with SEGFAULT that very hard to analyse even with GDB.</li>
    <li>Not very good compatibility with other implementations.</li>
</ul>
