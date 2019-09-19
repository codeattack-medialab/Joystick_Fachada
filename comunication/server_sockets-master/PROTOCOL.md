# Pachucho Protocol

## addPachucho

```
addPachucho ROOM
```

`addPachucho` message is sent by the client when it connects to the server. It pass as paramenter 'ROOM' that is the unique identifier of the room create for a pachucho and its pair.
`ROOM` is calculated in the client as follow:

> ROOM = Pachucho_1_ID + Pachucho_2_ID

## HUUUUUUUUUUUG

```
HUUUUUUUUUUUG number <number>
```

`HUUUUUUUUUUUG` message is sent when an event is generated in a client. This message is broadcasted in the room create for a couple of paired pachuchos. The number is a consecutive indicating the hug number; it is not used at the moment.
