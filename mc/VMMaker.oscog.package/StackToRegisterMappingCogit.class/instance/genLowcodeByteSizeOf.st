inline primitive generators generated code
genLowcodeByteSizeOf
	<option: #LowcodeVM>
	<inline: true>
	"Generated by the Lowcode instruction generator."
	| object value |
	self allocateRegistersForLowcodeOopResultInteger: [:objectValue :valueValue |
		object := objectValue.
		value := valueValue.
	].

	self ssTop popToReg: object.
	self ssPop: 1.

	self ssFlushAll.
	objectRepresentation genLcByteSizeOf: object to: value.

	^ 0

