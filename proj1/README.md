# Project 1 Distributed Key-Value store

CS737 Distributed Systems Fall 2019

# Design

These are components we do or may need:

1. Daemon (Sek)
2. IPC (Sek)
3. Backend data store (Sek) :white_check_mark:
4. Server-to-server communication protocol (Nick)
5. Load balancer / gateway
6. Monitoring / failure recovery system

## Backend Data Store

## Server Communication Protocol: SOSAD

The server communication protocol is a simple, one-sided, and dumb (SOSAD) protocol.  Each server-to-server message has three parts:

1. The timestamp of the most recently received update message.
2. The timestamp of the previous update message received from the destination server.
3. The contents of the most recent kvt (key, value, timestamp) changes for all key updates received since the previous update time.

   - The server does not echo all received messages, but only the latest key updates received for each key.  So if the server receives these updates:

     1. A 31
     2. B 11
     3. A 12
     4. C 39
     5. A 17
     6. C RL

   - The server will then echo these updates in a single update message:

     - A: 17 @ 5
     - B: 11 @ 2
     - C: RL @ 6

All messages are assumed to be complete and irrelevant: messages sent but never received or processed should have no detrimental effect on the system's state.  The local server should update the remote server's state based on the most recent message timestamp received from the remote server. E.g., if C received a message from A at time 5, but none of A's keys were newer than time 2, C would update A's most recent time to 2.

During syncs, servers are assumed to sync all keys as at least as recent as the remote server's state.  If C thinks A is at time 2, then it'll send A all keys with a timestamp of 2 or later.

### Sample Communication

During system initialization, all servers are initialized to the same update time wuth empty kv stores.

T=1:

| Time | Server | Key    | Value |
|------|--------|--------|-------|
| 1    | A      | B-now  | 1     |
| 1    | A      | B-prev | 1     |
| 1    | A      | C-now  | 1     |
| 1    | A      | C-prev | 1     |
| 1    | B      | A-now  | 1     |
| 1    | B      | A-prev | 1     |
| 1    | B      | C-now  | 1     |
| 1    | B      | C-prev | 1     |
| 1    | C      | B-now  | 1     |
| 1    | C      | B-prev | 1     |
| 1    | C      | A-now  | 1     |
| 1    | C      | A-prev | 1     |

T=2:

B receives updates from the client:

- B: waffle = yoda
- B: grace = jack

After this step's updates are applied, the servers look like this:

| Time | Server | Key      | Value |
|------|--------|----------|-------|
| 2    | A      | B-now    | 1     |
| 2    | A      | B-prev   | 1     |
| 2    | A      | C-now    | 1     |
| 2    | A      | C-prev   | 1     |
| 2    | B      | A-now    | 1     |
| 2    | B      | A-prev   | 1     |
| 2    | B      | C-now    | 1     |
| 2    | B      | C-prev   | 1     |
| 2    | B      | waffle:2 | yoda  |
| 2    | B      | grace:2  | jack  |
| 2    | C      | B-now    | 1     |
| 2    | C      | B-prev   | 1     |
| 2    | C      | A-now    | 1     |
| 2    | C      | A-prev   | 1     |

T=3:

B sends those updates to other servers, while C receives new updates:

- B -> A, C: waffle@2
- B -> A, C: grace@2
- C: frogger = munchkin

Unfortunately, A did not receive the update.

After this step's updates are applied, the servers look like this:

| Time | Server | Key       | Value    |
|------|--------|-----------|----------|
| 3    | A      | B-now     | 1        |
| 3    | A      | B-prev    | 1        |
| 3    | A      | C-now     | 1        |
| 3    | A      | C-prev    | 1        |
| 3    | B      | A-now     | 1        |
| 3    | B      | A-prev    | 1        |
| 3    | B      | C-now     | 1        |
| 3    | B      | C-prev    | 1        |
| 3    | B      | waffle:2  | yoda     |
| 3    | B      | grace:2   | jack     |
| 3    | C      | B-now     | 2        |
| 3    | C      | B-prev    | 1        |
| 3    | C      | A-now     | 1        |
| 3    | C      | A-prev    | 1        |
| 3    | C      | frogger:3 | munchkin |
| 3    | C      | waffle:2  | yoda     |
| 3    | C      | grace:2   | jack     |

T=4:

C syncs that update, but uh oh, A didn't receive the previous updates!  That's all right, we'll fix that shortly.  C also received another update, too.

- C -> A, B: frogger@3
- C: tom = turkey

After this step's updates are applied, the servers look like this:

| Time | Server | Key       | Value    |
|------|--------|-----------|----------|
| 4    | A      | B-now     | 1        |
| 4    | A      | B-prev    | 1        |
| 4    | A      | C-now     | 3        |
| 4    | A      | C-prev    | 1        |
| 4    | A      | frogger:3 | munchkin |
| 4    | B      | A-now     | 1        |
| 4    | B      | A-prev    | 1        |
| 4    | B      | C-now     | 3        |
| 4    | B      | C-prev    | 1        |
| 4    | B      | waffle:2  | yoda     |
| 4    | B      | grace:2   | jack     |
| 4    | B      | frogger:3 | munchkin |
| 4    | C      | B-now     | 2        |
| 4    | C      | B-prev    | 1        |
| 4    | C      | A-now     | 1        |
| 4    | C      | A-prev    | 1        |
| 4    | C      | frogger:3 | munchkin |
| 4    | C      | waffle:2  | yoda     |
| 4    | C      | grace:2   | jack     |
| 4    | C      | tom:4     | turkey   |

T=5:

Now, at T=5, all the servers sync keys they've received that are as new or newer than the remote server's state:

- A -> B, C: frogger@3
- B -> A: waffle@2, grace@2
- C -> A, B: frogger@3, tom@4

After this step's updates are applied, the servers look like this:

| Time | Server | Key       | Value    |
|------|--------|-----------|----------|
| 5    | A      | B-now     | 2        |
| 5    | A      | B-prev    | 1        |
| 5    | A      | C-now     | 4        |
| 5    | A      | C-prev    | 1        |
| 5    | A      | frogger:3 | munchkin |
| 5    | A      | waffle:2  | yoda     |
| 5    | A      | grace:2   | jack     |
| 5    | A      | tom:4     | turkey   |
| 5    | B      | A-now     | 3        |
| 5    | B      | A-prev    | 1        |
| 5    | B      | C-now     | 4        |
| 5    | B      | C-prev    | 1        |
| 5    | B      | tom:4     | turkey   |
| 5    | B      | waffle:2  | yoda     |
| 5    | B      | grace:2   | jack     |
| 5    | B      | frogger:3 | munchkin |
| 5    | C      | B-now     | 2        |
| 5    | C      | B-prev    | 1        |
| 5    | C      | A-now     | 3        |
| 5    | C      | A-prev    | 1        |
| 5    | C      | frogger:3 | munchkin |
| 5    | C      | waffle:2  | yoda     |
| 5    | C      | grace:2   | jack     |
| 5    | C      | tom:4     | turkey   |

### Possible Optimizations

1. Suppress timestamps in update messages and use only the most recent timestamp (or the average timestamp) as the keys' changed times.
2. Make smaller update messages by storing each other server and version with each key.  That way we can send exactly the keys the other server needs to catch up, but is somewhat more complex to implement and takes more memory per key to store.
3. Send full-sync messages on a rolling basis, sending them at different times per server.

#2 is probably the most important improvement for reliability if message delivery is not guaranteed.

## Desirable Properties

### Frontend

- A frontend that load balances by sending results to several servers.
- A frontend that hands off packets to the backends directly, so clients never communicate directly to the backend.

### Backend

- A distributed backend datastore that communicates between servers to find data.

## Some considerations

- Should sync happen at the read, or write?

### read

Find the latest copy among the servers send it to the requester, all other client update there stale copy

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
