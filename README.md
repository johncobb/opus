# Encoding workflow tasks/sub-taks with 16/32/64 bit integers

The goal of this project is to experiment with bit encoding tasks and relating sub-tasks which represent predefined data flows within a workflow system. Optimizing the data structure allows for more efficient transmission and processing of tasks. 

The example we will use in this project is tracking food preparation steps.

Example: Task 1/Spaghetti
1: Boil Noodles
2: Cook Meat
3: Add Sauce
4: Serve

Example: Task 2/Pancakes
1: Pour Mix
2: Add Eggs
3: Add Milk
4: Mix Thoroughly
5: Cook to golden brown
6: Serve

Now let's aassume we were tasked to create a system for managing  chefs in a restaurant. The restaurant manager would like to send the tasks to each chef and monitor the progress as each step of the preparatin process is completed. Simple you say, yeah, I agree, a few tables in a database and we have the start of a  workflow tracking system.

## Conventional Wisdom
Conventional wisdom says create relational tables to represent the flow.
```
Table: Tasks
Id INT
Name CHAR(20)

Table: SubTasks
SubTaskId
TaskId
CHAR(20)

Table: TaskWorkflow:
Id INT
TaskId INT
CurrentSubTaskStep INT
ChefId INT
Timestamp DATETIME
```

We now have the tables needed to identify all the tasks and their relating sub-tasks. We also have a table to track the workflow as each chef is assigned tasks and begins working through each subtask/step to completion. This is great but as you can see the data starts to add up over time. What's more is the level of joining and lookups required to track each task as well as calculate the timing of each task can get pretty resource intensive. In a system where there is a lot of transaction volume the overhead starts to accumulate quickly. Indexing this system while not imposssible is a balance of designing for query speed and write performance. A somewhat paradoxical initiative in that one tends to mute out the other.


## Less is More
The tasking system I am proposing is optimied by both identifying the task and managing its sub-task state in as little as a two byte integer called a short. The 8 high order bits represent the task identifier and the 8 low order bits represent the relating sub-tasks or steps involved to complete the task. For our simple 2 byte tracking system this gives us a scope of 256 tasks containing up to 256 sub-tasks.

Listed below is an example of a task with sub-task 1 active. The task id is calculate by masking out the 8 low order bits and dividing the 8 high order bits by 256. Doing so yields the task id.


Mask the 8 low order bits:
```
    0000 0001 0000 0001
AND
    1111 1111 0000 0000
EQUALS
    0000 0001 0000 0000
EQUALS
    256
EQUALS
    256/256=1
```


Example:
Spaghetti:
```
0000 0001 0000 0000 = 256/256 = 1
```
Pancakes:
```
0000 0010 0000 0000 = 512/256 = 2
```
The same is done to the low order bits to calculate the sub-task id.

## Performance/Optimization
The novelty of this system is the deterministic nature of the high and low order bits. Shrinking the representation of 256 tasks/sub-tasks to 2 bytes. Transmitting this data over the wire is extremely efficient and the logic operations to calculate values require minimal computation. The integer optimization also allows for more efficient data storage and indexing in traditional RDBMS systems. Overall query and write performance is optimal.

Tracking the workflow and describing each task can be done with two tables. A third journaling table could be added to track the transient states of each task.
```
Table TaskWorkflow:
TaskId
UserId
Epoch
```

The tasks and subasks can be described in the same table due to the nature of the unique combination of bits representing the task/sub-task.
```
Table TaskMeta:
Id
Label
```


## Future
The system can be expanded using the signed bit to determine whether a task is complete. If the task meta defines a max task value the task could be autocompleted or closed out by flipping the signed bit. This adds an intrinsic querying facility that can quickly query all tasks.

## Exploratory
32 and 64 bit variants allowing permission scope and possible lock-step/parallel task triggering.
