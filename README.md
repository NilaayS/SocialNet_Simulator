# SocialNet Simulator

**Author: Nilay Surana**

---

## Project Overview

The objective of this assignment is to build **SocialNet**, a command-line simulator for a social network's backend services. The system manages users, friendships, and content by applying AVL Trees and Graph data structures.

Key features include:

- Adding users  
- Establishing bidirectional friendships  
- Suggesting friends based on mutual connections  
- Calculating degrees of separation  
- Managing user posts in chronological order  

---

## How to Compile and Run

### Windows (MinGW)

For running on a Windows device with MinGW installed:

1. Open PowerShell  
2. Enter the project directory  
3. Compile the program:

```bash
g++ -std=c++14 interface.cpp -o interface.exe
```

4. Run:

```bash
./interface.exe
```

---

### macOS / Linux

1. Open Terminal  
2. Enter the project directory  
3. Grant execute permission:

```bash
chmod +x compile.sh
```

4. Compile:

```bash
./compile.sh
```

5. Execute:

```bash
./interface
```

---

### IMPORTANT NOTE

Although only `interface.cpp` needs to be compiled, it uses `structures.cpp` through `#include`. Therefore, both files must remain in the same directory.

Two separate files have been made purely for convenience and decluttering, and for no other reason.

---

## Inputs and Error Handling

Once the program starts, it dynamically accepts inputs and performs the necessary actions immediately after each command is entered.

To stop the program, enter:

```
STOP
```

(All caps required.)

---

# Expected Inputs

---

## Network-Based Inputs

### Add User

```
ADD_USER username
```

Sample input:

```
ADD_USER ramu
```

Creates a user named `ramu`.

- Username can be any string  
- Two users with the same name (case insensitive) cannot be created  
- If a duplicate is attempted, the program displays an error message  

---

### List Friends

```
LIST_FRIENDS username
```

Sample input:

```
LIST_FRIENDS ramu
```

Lists all friends of `ramu`.

---

### Suggest N Friends

```
SUGGEST_FRIENDS username N
```

Sample input:

```
SUGGEST_FRIENDS ramu N
```

- Suggests `N` friends based on the number of mutual friends  
- Only users with at least one mutual friend are suggested  
- If `N` is greater than available probable friends, all probable friends are displayed  

---

### Degrees of Separation

```
DEGREES_OF_SEPARATION user1 user2
```

Sample input:

```
DEGREES_OF_SEPARATION ramu shyamu
```

- Outputs the degrees of separation between the two users  
- If `user1 = user2`, the output is `0`  

---

## User-Based Inputs

---

### Add Post

```
ADD_POST username content
```

Sample input:

```
ADD_POST ramu hello this is my first post
```

Adds a post to `ramu` with `"hello this is my first post"` as its content.

---

### Output N Posts

```
OUTPUT_POSTS username N
```

Sample input:

```
OUTPUT_POSTS ramu 1
```

- Outputs `ramu`'s `N` most recent posts  
- If `N` is greater than the number of posts, all posts are displayed  
- If `N = -1`, all posts are displayed  

---

# Error Handling

The program handles invalid inputs and provides informative feedback.

### Invalid Input
- Wrong command format  
- Unknown command name  
- Incorrect number of arguments  

### Invalid Input: N Must Be A Non -ve Integer
- If the number `N` provided to `SUGGEST_FRIENDS` is not a valid non-negative integer  

### Invalid Input: N Must Be A Non -ve Integer other than -1
- If the number `N` provided to `OUTPUT_POSTS` is not a valid non-negative integer or not `-1`  

### Please Enter 2 Different Usernames
- If the same valid usernames are entered in `ADD_FRIEND`  

### Either Of The Usernames Or Both Of Them Don't Exist
- If `ADD_FRIEND` or `DEGREES_OF_SEPARATION` is called and one or both users do not exist  

### [user1] and [user2] are already friends
- If `ADD_FRIEND` is called on two users who are already connected  

---

# Data Structures and Methods Implemented

All core data structures (Graphs and AVL Trees) were implemented from scratch as required, without using C++ standard library containers for the primary logic.

---

## Network (Graph)

The entire social network is represented as an undirected graph:

- Each user is a vertex  
- Each friendship is an edge  

The `Network` class holds all `User` objects (vertices).

### User Lookup

The `Network` class uses a C++ `unordered_map` (as permitted) to map lowercase username strings to their corresponding `User*` objects. This provides **O(1)** average-time access to any user.

### Adjacency

An adjacency list model is used, where each `User` (vertex) stores its list of friends in a dedicated AVL tree (`ftree`).

---

## ftree (AVL Tree)

This custom self-balancing AVL tree is used by each `User` object to store pointers to their friends (other `User` objects).

### Purpose

- Maintains the friend list sorted alphabetically by username  
- Allows efficient, sorted retrieval for the `LIST_FRIENDS` command  

### Operations

- Self-balancing insert (using LL and RR rotations)  
- In-order traversal (`listfriends`) to output the sorted list  

---

## userposts (AVL Tree)

A second custom self-balancing AVL tree is used by each `User` object to store their posts.

### Purpose

- Efficiently stores posts sorted by time of creation  

### Operations

- Balancing insert  
- Reverse in-order traversal (`output`) to fetch the `N` most recent posts in reverse chronological order  

---

## UNode & pnode (Tree Nodes)

The fundamental nodes for the two AVL tree implementations.

### UNode

- Node in the `ftree`  
- Holds a `User*` (friend pointer)  
- Contains parent/child pointers  
- Stores height and balance information  

### pnode

- Node in the `userposts` tree  
- Holds a `post*` (post pointer)  
- Contains parent/child pointers  
- Stores height and balance information  

---

## User::friendlist (HashMap)

In addition to the `ftree`, each `User` object also contains a C++ `unordered_map` to provide an **O(1)** average-time duplicate check before adding a new friend.

This ensures efficient validation while maintaining sorted storage through the AVL tree.
