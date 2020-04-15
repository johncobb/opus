
## Streaming API
API architecture for data model abstraction


```console
endpoint: vinpoint.io

/* traditional */
/api/org/:orgId/entity/:entityId
/api/org/:orgId/user/:userId
/api/org/:orgId/entity/:entityId/location/:locationId
/api/org/:orgId/entity/:entityId/asset/:assetId
/api/org/:orgId/entity/:entityId/asset/:assetId/tag/:tagId
/api/org/:orgId/entity/:entityId/asset/:assetId/task/:taskId


/api/org/:orgId/
/api/org/:orgId/entity
/api/org/:orgId/user
/api/org/:orgId/entity/:entityId/asset
/api/org/:orgId/entity/:entityId/user
/api/org/:orgId/entity/:entityId/location

System Heirarchy
org:
 - entity
 - user
 - permission
 - asset
 - location
 - attribute
 - task

Unique Characteristics
A tag is intended to be a polymorphic structure
that can represent multiple types.
tag:
 - permission
 - group
 - location
 - attribute

group:
 - grouping users (access levels)
 - grouping entities (district/regional)
 - grouping permissions
Group Example:
The admin group defines the following permisisons.
The East District group encompasses these dealerships

Assignment
org:
 - assigned entities
 - assigned users
 - assigned assets (can it?)

entity:
 - assigned users
 - assigned groups (district/regional)
 - assigned assets
 - assigned locations

user:
 - assigned permissions
 - assigned tasks

asset:
 - assigned attributes
 - assigned tasks


tags can be assigned to
 - multiple assets
 - asasets can be assigned multiple tags
 - tags can be used to group assets


tag:
 - attribute
 - group
 - permission

org:
 - user
   - tasks
     - task-id: 0x100
   - tasks-permission
     - task-id: 0x100
      - task-id-subtask-1: 0x110
      - task-id-subtask-2: 0x210
      - task-id-subtask-3: 0x310
 - asset
   - tag-group-1
   - tag-group-2
 - entity - (boundary)
   - tag-group
   - user
     - tag-group-1
   - tag-location (boundary)
   - asset (boundary)
     - task

/* entity model abstraction */
entity:
  - tag-region(n)
  - tag-district(n)
  - tag-user(n)
  - tag-location(n)
/* entity model implementation */
entity-fort-lauderdale
  - tag-east
  - tag-south-florida
  - tag-scanner-user




asset:
  - tag-group
  - tag-user



tag:
  - id
  - name
  - lat, lng (boundary)

/* asset inheritance of tag */
asset:tag
  - id
  - name
  - lat, lng
    - identifier
    -  



EntityMeta
 : Entityid
 : Name
 : Address
 : City
 : State
 : Zip
 : Lat
 : Lng

```

YouTube: https://www.youtube.com/watch?v=O_jNJ32s6x8
Redis notes:

Storing user session in hash
Queues for high performance/scaling
Redis /Sorted Set
Search Geographical Area/Geospatial Indexes
Pub/Sub 9:30 approx.
Reliable Messaging Redis Streams 25:00 approx.

