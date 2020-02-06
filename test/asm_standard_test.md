test/asm_standard_test.mc AST:
{
| {
| | type: FUNC
| | funcType: INT
| | isPtr: 0
| | isStatic: 0
| | funcName: main
| | 
| | scope:
| | {
| | | {
| | | | type: VAR
| | | | varName: b
| | | | varType: INT
| | | | isPtr: 0
| | | | isStatic: 0
| | | | isArray: 0
| | | },
| | | {
| | | | type: ASSIGN
| | | | varName:
| | | | {
| | | | | {
| | | | | | type: ID
| | | | | | isPtr: 0
| | | | | | varName: b
| | | | | },
| | | | },
| | | | 
| | | | exprsn:
| | | | {
| | | | | {
| | | | | | type: LIT
| | | | | | type: INT
| | | | | | val: 0
| | | | | },
| | | | },
| | | },
| | },
| },
| {
| | type: STRUCT
| | strctName: a
| | childrenSz: 1
| | 
| | decls:
| | {
| | | {
| | | | type: SCOPE
| | | },
| | | {
| | | | type: VAR
| | | | varName: a
| | | | varType: BYTE
| | | | isPtr: 0
| | | | isStatic: 0
| | | | isArray: 0
| | | },
| | },
| },
},
{
| type: FUNC
| funcType: CUSTOM 3
| isPtr: 0
| isStatic: 0
| funcName: foo
},
},
