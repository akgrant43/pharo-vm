debug printing
printSortedFreeList
	<api>
	| freeChunk prevFree nextFree |
	(firstFreeChunk > 0 and: [lastFreeChunk > firstFreeChunk]) ifFalse:
		[coInterpreter print: 'sorted free list empty or corrupt'; cr.
		 ^self].
	freeChunk := firstFreeChunk.
	prevFree := 0.
	[((manager addressCouldBeObj: freeChunk)
	 and: [manager isFreeObject: freeChunk]) ifFalse:
		[coInterpreter printHexnp: freeChunk; print: ' is not a free chunk!' ; cr.
		 ^self].
	 manager printFreeChunk: freeChunk printAsTreeNode: false.
	 freeChunk ~= lastFreeChunk] whileTrue:
		[nextFree := self nextInSortedFreeListLink: freeChunk given: prevFree.
		 prevFree := freeChunk.
		 freeChunk := nextFree]