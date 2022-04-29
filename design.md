This file is currently out of date.
## Considerations

- The State holds all mutable values that we need to stabilize, so the state should contain the stabilize function.
- We need to be able to get the actual value from an Anchor, as well as enforce that it is up to date.

- There should be an engine class which manages the state.

### Engine
The engine class here performs a similar function to incremental's state. 

- get(anchor) -> Given an anchor, call state.stabilize(), then return the value of the anchor. Maybe only stabilize if it is an observed node.
- observe(anchor) -> Given an anchor, do all the work that needs to be done when you mark a node as observed, i.e. visiting its children to add the new parents. Mark the children as necessary too. 
- unobserve(anchor).
- stabilize: only compute necessary nodes. Nodes in the recompute heap. Maintain the invariant that any value that needs recomputing will be in the recompute heap.
- How do we coordinate set_updater with the adjust heights heap? (for later)
- maybe the engine can be responsible for the coordination? 

 A node is "stale" if it has never been computed or if its recompute id is less than the change id of one of its children.

### Anchor
- Users can create an Anchor in a number of ways:
    - Just provide the value in a constructor.
    - Static functions like:
        - `map (Anchor<A> anchor, std::function<Anchor<T>>(A val)) // function that takes an Anchor and a function that tells it how to create an Anchor from the value of the Anchor.`
        - `map2(Anchor<A> anchorA, Anchor<B> anchorB, std::function<T>(A val, B val))`
      
- When you create an Anchor, you create it with a height. Its height must always be greater than those of its input.
- The above functions won't do any computations until you call get.
- set(). - When called, we need to be able to access its parents and add them to recompute heap
- In the map functions, the inputs A and B become children of the new anchor. We also store the updater function.
- get() - will just run the updater function and return a value. This should work well depending we maintain the invariant that children always get computed before their parents. 

We should store the value of each run and only run the updater function if its input has changed. 

Alternatively, we could have a different compute() function which just stores a new value, while get() always gives you the latest value.

- Does this mean there will always be a maximum of two children? We can know whether to use a map1 or map2 function by the number of children it has.

- Maybe also add support for caching. If the inputs to the updater function have not changed, then we don't need to recompute them.

- Make an anchor hashable. What makes two anchors the same?

- How do we store anchors in a container given that they might be of different types?

For later
- bind()
- Must every anchor take an input from other anchors? I think so 
- Start with only allowing inputs to be of the same type.
- setUpdater() - to update the map function. We need to look at the heights of input here. This is where the adjustHeights heap comes in. You also need to remove existing edges.
- Maybe instead of pointers, I can implement the right move functions




### Phase 1
The first goal is to implement the basic functionality of Incremental with tests, which includes:
	- User can create Anchors from scratch or from an existing anchor either using map or then.
	- User can observe an anchor.
	- User can get the value of an anchor, only recomputing when necessary. 
    - Do some performance tests.

I should be able to test this with a single add function