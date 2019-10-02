# Joystick Protocol

## addJoystick

```
addJoystick ROOM
```

`addJoystick` message is sent by the client when it connects to the server. It pass as paramenter 'ROOM' that is the unique identifier of the room create for a pachucho and its pair.
`ROOM` is calculated in the client as follow:

> ROOM = "8"

```
Left-1.0 number <number>
```
```
Right-1.0 number <number>
Right-0.0 number <number>
```
```
Click-1.0 number <number> 
Click-0.0 number <number>
```
Number is the idJoystick, and 1.0 is Pressed, and 0.0 represents Released

If this server receive this message then it will be send again by UDP sockets using PORT 33333 to GODOT. ( or another UDP server listener app )
