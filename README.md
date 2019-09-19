# CS739 Fall 2019

Project page for CS739 - Distributed Systems

## Project 1 Distributed Key-Value Store

 - [Files](https://github.com/sekcheong/cs739_2019/tree/master/proj1)

# Design

## Components

These are components we do or may need:

1. Demon
2. IPC (Sek)
3. Backend data store (Sek) :white_check_mark:
4. Server-to-server communication protocol (Nick)
5. Load balancer / gateway
6. Monitoring / failure recovery system

## Tests

1. Volume
   1. Volume per second
   2. Total volume
2. Consistency:
   1. Do we get the data we set back?
   2. What if we haven't set any data?
   3. Do we get old data back?
   4. How long before we get the right data back from each server?
3. Killing Servers
4. Malformed Requests
   1. Requests whose keys are too long.
   2. Requests whose values are too long.
   3. Requests whose keys are made from invalid characters (>128 or contain `[]`).
   4. Requests whose values are made from invalid characters.
   5. Requests without keys.
   6. Requests without values.
   7. Requests with multiple keys.
   8. Requests with multiple values.
   9. Requests with only keys.
   10. Requests with only values.
   11. Requests that begin in one packet but are never completed in another packet.
5. Random key read.
   1. Requesting keys that aren't there.
   2. Requesting all the keys.
6. Sending packets before sending a response: SYN flood.
7. Cache hit test.
8. Misordered request tests.
9. Mistimed request tests.
10. Kill the load balancer.

## Server to Server Communication Protocol

It should be straightforward to develop an eventually-consistent system.  This lets each server send its fellows a copy of the changes as they happen or in batch.  We could combine that with heartbeats to send batches of K/V updates and latest time stamps.  That way, if a server receives a heartbeat with the latest value from the past, it could reply with its own heartbeat containing any necessary updates.

## Desirable Properties

### Frontend

- A frontend that load balances by sending results to several servers.
- A frontend that hands off packets to the backends directly, so clients never communicate directly to the backend.

### Backend

- A distributed backend datastore that communicates between servers to find data.
