sista support
allImmediate: immediateMask branchIf: reg notInstanceOfBehaviors: arrayObj target: targetFixUp
	| jmpImmediate |
	< inline: true>	
	self assert: immediateMask = objectMemory tagMask.
	cogit MoveR: reg R: TempReg.
	jmpImmediate := self genJumpNotImmediateInScratchReg: TempReg.
	jmpImmediate jmpTarget: targetFixUp.
	^0