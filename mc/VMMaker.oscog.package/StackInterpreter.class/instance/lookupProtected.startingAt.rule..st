message sending
lookupProtected: selector startingAt: mixinApplication rule: rule
	"A shared part of the lookup for self, outer or implicit receiver sends that did not find a
	private lexically visible method, and (Newspeak) super sends."
	| lookupClass dictionary found |
	lookupClass := mixinApplication.
	[lookupClass = objectMemory nilObject] whileFalse:
		[dictionary := objectMemory followObjField: MethodDictionaryIndex ofObject: lookupClass.
		found := self lookupMethodInDictionary: dictionary.
		(found and: [(self accessModifierOfMethod: newMethod) ~= AccessModifierPrivate])
			ifTrue: [^rule].
		lookupClass := self superclassOf: lookupClass].
	^self lookupDnuAbsent