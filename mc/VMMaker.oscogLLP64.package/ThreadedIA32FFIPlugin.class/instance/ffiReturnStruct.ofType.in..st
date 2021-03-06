callout support
ffiReturnStruct: longLongRet ofType: ffiRetType in: calloutState
	<var: #longLongRet type: #usqLong>
	<var: #calloutState type: #'CalloutState *'>
	"Create a structure return value from an external function call.  The value as been stored in
	 alloca'ed space pointed to by the calloutState."
	| retOop retClass oop |
	<inline: true>
	retClass := interpreterProxy fetchPointer: 1 ofObject: ffiRetType.
	retOop := interpreterProxy instantiateClass: retClass indexableSize: 0.
	self remapOop: retOop
		in: [oop := interpreterProxy 
					instantiateClass: interpreterProxy classByteArray 
					indexableSize: calloutState structReturnSize].
	(self returnStructInRegisters: calloutState structReturnSize)
		ifTrue:
			[self mem: (interpreterProxy firstIndexableField: oop) cp: (self addressOf: longLongRet) y: calloutState structReturnSize]
		ifFalse:
			[self mem: (interpreterProxy firstIndexableField: oop) cp: calloutState limit y: calloutState structReturnSize].
	interpreterProxy storePointer: 0 ofObject: retOop withValue: oop.
	^interpreterProxy methodReturnValue: retOop